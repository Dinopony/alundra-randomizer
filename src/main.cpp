//////////////////////////////////////////////////////////////////////////////////////////
//
//     ALUNDRA RANDOMIZER
//
// ---------------------------------------------------------------------------------------
//
//     Developed by: Dinopony (@DinoponyRuns)
//
//////////////////////////////////////////////////////////////////////////////////////////

#include <string>
#include <iostream>

#include "personal_settings.hpp"
#include "randomizer_options.hpp"

#include "model/randomizer_world.hpp"
#include "world_shuffler.hpp"
#include "io/io.hpp"

#include "tools/argument_dictionary.hpp"
#include "tools/exception.hpp"
#include "tools/json.hpp"
#include "tools/base64.hpp"
#include "tools/binary_file.hpp"
#include "patches/patches.hpp"
#include "tools/psxexe_file.hpp"

void dump_iso(const std::filesystem::path& input_path, const std::filesystem::path& output_dir)
{
#ifdef WIN32
    std::string command = "tools\\dumpsxiso.exe \"" + input_path.string() + "\"";
#else
    std::string command = "./tools/dumpsxiso \"" + input_path.string() + "\"";
#endif
    command += " -x \"" + output_dir.string() + "\"";
    command += " -s \"" + output_dir.string() + "/build.xml\"";

    // Remove standard output for this command
#ifdef WIN32
    command += " > nul";
#else
    command += " > /dev/null";
#endif

    system(command.c_str());
}

void rebuild_iso(const std::filesystem::path& input_dir, const std::filesystem::path& output_file)
{
    std::filesystem::path cue_file_path = output_file;
    cue_file_path.replace_extension("cue");

#ifdef WIN32
    std::string command = "tools\\mkpsxiso.exe \"" + input_dir.string() + "build.xml\"";
#else
    std::string command = "./tools/mkpsxiso \"" + input_dir.string() + "build.xml\"";
#endif

    command += " -o \"" + output_file.string() + "\"";
    command += " -c \"" + cue_file_path.string() + "\"";

    // Remove standard output for this command
#ifdef WIN32
    command += " > nul";
#else
    command += " > /dev/null";
#endif

    system(command.c_str());
}

void process_paths(const ArgumentDictionary& args, const RandomizerOptions& options, std::string& output_rom_path, std::string& spoiler_log_path)
{
    // TODO: Rewrite this with the <filesystem> standard lib
    output_rom_path = args.get_string("outputrom", "./");
    spoiler_log_path = args.get_string("outputlog", "./");

    // Clean output ROM path and determine if it's a directory or a file
    if(output_rom_path.empty())
        output_rom_path = "./";

    bool output_path_is_a_directory = !output_rom_path.ends_with(".cue") && !output_rom_path.ends_with(".bin");
    if(output_path_is_a_directory && *output_rom_path.rbegin() != '/')
        output_rom_path += "/";

    // If output log path wasn't specified, put it along with the ROM
    if(!args.contains("outputlog"))
    {
        // outputRomPath points to a directory, use the same for the spoiler log
        if(output_path_is_a_directory)
            spoiler_log_path = output_rom_path;

        // outputRomPath points to a file, change its extension to ".json"
        else if(output_rom_path.ends_with(".cue"))
            spoiler_log_path = output_rom_path.substr(0, output_rom_path.size() - 2) + "json";
        else if(output_rom_path.ends_with(".bin"))
            spoiler_log_path = output_rom_path.substr(0, output_rom_path.size() - 3) + "json";
    }
    else if(!spoiler_log_path.empty() && !spoiler_log_path.ends_with(".json") && !spoiler_log_path.ends_with('/'))
        spoiler_log_path += "/";

    // Add the filename afterwards if required
    if(!output_rom_path.empty() && *output_rom_path.rbegin() == '/')
        output_rom_path += options.hash_sentence() + ".bin";
    if(!spoiler_log_path.empty() && *spoiler_log_path.rbegin() == '/')
        spoiler_log_path += options.hash_sentence() + ".json";
}

Json randomize(RandomizerWorld& world, RandomizerOptions& options, PersonalSettings& personal_settings, const ArgumentDictionary& args)
{
    Json spoiler_json;

    spoiler_json["permalink"] = options.permalink();
    spoiler_json["hashSentence"] = options.hash_sentence();
    spoiler_json.merge_patch(options.to_json());

    // Apply randomizer options to alter World and RandomizerWorld accordingly before starting the actual randomization
    // apply_randomizer_options(options, world);

    // Parse a potential "world" section inside the preset for plandos & half plandos
    // WorldJsonParser::parse_world_json(world, options.world_json());

    // In rando mode, we rock our little World and shuffle things around to make a brand new experience on each seed.
    std::cout << "\nRandomizing world...\n";
    WorldShuffler shuffler(world, options);
    shuffler.randomize();

    if(options.allow_spoiler_log())
    {
        spoiler_json.merge_patch(SpoilerWriter::build_spoiler_json(world, options));
        spoiler_json["playthrough"] = shuffler.playthrough_as_json();

        // Output debug log if requested, only if spoiler log is authorized
        std::string debug_log_path = args.get_string("debuglog");
        if (!debug_log_path.empty())
        {
            std::ofstream debug_log_file(debug_log_path);
            debug_log_file << shuffler.debug_log_as_json().dump(4);
            debug_log_file.close();
        }

#ifdef DEBUG
        // Output model if requested, only if spoiler log is authorized
        if(args.get_boolean("dumpmodel"))
        {
            std::cout << "Outputting model...\n\n";
            ModelWriter::write_logic_model(world);
            std::cout << "Model dumped to './json_data/'" << std::endl;
        }
#endif
    }

    return spoiler_json;
}

void generate(const ArgumentDictionary& args)
{
    RandomizerWorld world;
    world.load_model_from_json();

    RandomizerOptions options(args, world.item_names());
    world.apply_options(options);

    PersonalSettings personal_settings(args);

    if(args.contains("graph"))
    {
        GraphvizWriter::write_logic_as_dot(world, "./logic.dot");
    }

    Json spoiler_json = randomize(world, options, personal_settings, args);

    // Parse output paths from args
    std::string output_image_path, spoiler_log_path;
    process_paths(args, options, output_image_path, spoiler_log_path);

    if(!args.contains("only-logic"))
    {
        std::string input_rom_path = args.get_string("input", "./input.bin");

        if(!std::filesystem::exists("./base_dump/DATA/DATAS.BIN"))
        {
            BinaryFile image_file(input_rom_path);
            uint64_t checksum = image_file.checksum();
            if(checksum != 51532940438)
                throw RandomizerException("Invalid checksum (" + std::to_string(checksum) + ") on the image file. Make sure you are using a 1.1 US image.");

            std::cout << "Performing the initial image dump (" << checksum << ")...\n\n";
            dump_iso(input_rom_path, "./base_dump/");
        }

        std::cout << "Loading game files...\n";
        std::filesystem::remove_all("./tmp_dump/");
        std::filesystem::copy("./base_dump/", "./tmp_dump/", std::filesystem::copy_options::recursive);

        BinaryFile datas_file("./tmp_dump/DATA/DATAS.BIN");
        PsxExeFile exe_file("./tmp_dump/ALUN_CD.EXE");

        // Apply patches to the game ROM to alter various things that are not directly part of the game world randomization
        std::cout << "Applying game patches...\n";
        apply_randomizer_patches(datas_file, exe_file, world, options);

        // Save the edited DATAS.BIN and ALUN_CD.EXE files to disk
        std::cout << "Writing edited files to disk...\n\n";
        datas_file.save();
        exe_file.save();

        std::cout << "Building a disc image...\n\n";
        rebuild_iso("./tmp_dump/", output_image_path);
#ifndef DEBUG
        std::filesystem::remove_all("./tmp_dump/");
#endif

        std::cout << "Randomized game outputted to \"" << output_image_path << "\".\n";
    }
    
    // Write a spoiler log to help the player
    if(!spoiler_log_path.empty())
    {
        std::ofstream spoiler_file(spoiler_log_path);
        if(!spoiler_file)
            throw RandomizerException("Could not open output log file for writing at path '" + spoiler_log_path + "'");

        spoiler_file << spoiler_json.dump(4);
        spoiler_file.close();
        if(options.allow_spoiler_log())
            std::cout << "Spoiler log written into \"" << spoiler_log_path << "\".\n";
        else
            std::cout << "Generation log written into \"" << spoiler_log_path << "\".\n";
    }

    std::cout << "\nHash sentence: " << options.hash_sentence() << "\n";
    std::cout << "\nPermalink: " << options.permalink() << "\n";
    std::cout << "\nShare the permalink above with other people to enable them building the exact same seed.\n" << std::endl;
}

int main(int argc, char* argv[])
{
    int return_code = EXIT_SUCCESS;

    ArgumentDictionary args(argc, argv);

    std::cout << "======== Alundra Randomizer v" << RELEASE << " ========\n\n";

    if(args.contains("permalink") && args.get_string("permalink").empty())
    {
        std::string permalink;
        std::cout << "Please specify a permalink: ";
        std::getline(std::cin, permalink);
        args.set_string("permalink", permalink);
    }

    try
    {
        int seed_count = args.get_integer("seedcount", 1);
        for(int i=0 ; i<seed_count ; ++i)
            generate(args);
    }
    catch(RandomizerException& e)
    {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return_code = EXIT_FAILURE;
    }

    if(args.get_boolean("pause", true))
    {
        std::cout << "\nPress any key to exit.";
        std::string dummy;
        std::getline(std::cin, dummy);
    }

    return return_code;
}
