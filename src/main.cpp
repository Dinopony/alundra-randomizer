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

#include "game/game_data.hpp"
#include "model/randomizer_world.hpp"
#include "world_shuffler.hpp"
#include "io/io.hpp"

#include "tools/argument_dictionary.hpp"
#include "tools/exception.hpp"
#include "tools/json.hpp"
#include "tools/base64.hpp"
#include "tools/binary_file.hpp"
#include "tools/psx_exe_file.hpp"
#include "patches/patches.hpp"

/**
 * Calls the external tool `dumpsxiso` in order to dump the game image into a folder containing
 * all game files.
 * 
 * @param input_file_path the path to the disc image file
 * @param output_dir_path the path to the output directory where game files will be extracted
 */
void dump_iso(const std::filesystem::path& input_file_path, const std::filesystem::path& output_dir_path)
{
#ifdef WIN32
    std::string command = "tools\\dumpsxiso.exe \"" + input_file_path.string() + "\"";
#else
    std::string command = "./tools/dumpsxiso \"" + input_file_path.string() + "\"";
#endif
    command += " -x \"" + output_dir_path.string() + "\"";
    command += " -s \"" + output_dir_path.string() + "/build.xml\"";

    // Remove standard output for this command
#ifdef WIN32
    command += " > nul";
#else
    command += " > /dev/null";
#endif

    system(command.c_str());
}

/**
 * Calls the external tool `mkpsxiso` in order to re-pack the game image from a folder containing
 * all game files.
 * 
 * @param input_dir_path the path to the directory containing game files
 * @param output_file_path the path to the output disc image file that will be created
 * @return true if the process succeeded, false otherwise
 */
bool rebuild_iso(const std::filesystem::path& input_dir_path, const std::filesystem::path& output_file_path)
{
    std::filesystem::path cue_file_path = output_file_path;
    cue_file_path.replace_extension("cue");

#ifdef WIN32
    std::string command = "tools\\mkpsxiso.exe \"" + input_dir_path.string() + "build.xml\"";
#else
    std::string command = "./tools/mkpsxiso \"" + input_dir_path.string() + "build.xml\"";
#endif

    command += " -o \"" + output_file_path.string() + "\"";
    command += " -c \"" + cue_file_path.string() + "\"";
    command += " -y";

    // Remove standard output for this command
#ifdef WIN32
    command += " > nul";
#else
    command += " > /dev/null";
#endif

    int exit_code = system(command.c_str());
    return exit_code == 0;
}

/**
 * Process the given output paths (input by the user) to alter them following a bunch of rules.
 * 
 * @param output_rom_path a reference on the path that will be used for the output ROM
 * @param spoiler_log_path a reference on the path that will be used for the spoiler log
 * @param hash_sentence the seed unique "hash sentence", used as a default filename if none was given
 */
void process_paths(std::filesystem::path& output_rom_path, std::filesystem::path& spoiler_log_path,
                   const std::string& hash_sentence)
{
    // If output ROM path was not specified, put it in the current working directory
    if(output_rom_path.empty())
        output_rom_path = "./";
    
    // If output log path wasn't specified, put it alongside the ROM
    if(spoiler_log_path.empty())
    {
        spoiler_log_path = output_rom_path;
        if(spoiler_log_path.has_extension())
            spoiler_log_path.replace_extension(".json");
    }

    // If path was not containing the appropriate file extension, it is considered as a directory path,
    // so append a default filename to it.
    if(output_rom_path.extension() != ".bin")
        output_rom_path = output_rom_path / (hash_sentence + ".bin");
    if(spoiler_log_path.extension() != ".json")
        spoiler_log_path = spoiler_log_path / (hash_sentence + ".json");
}

Json randomize(RandomizerWorld& world, GameData& game_data, RandomizerOptions& options, PersonalSettings& personal_settings, const ArgumentDictionary& args)
{
    Json spoiler_json;

    spoiler_json["permalink"] = options.permalink();
    spoiler_json["hashSentence"] = options.hash_sentence();
    spoiler_json.merge_patch(options.to_json(game_data, world));

    std::cout << "\nRandomizing world...\n";
    WorldShuffler shuffler(world, game_data, options);
    shuffler.randomize_items();
    shuffler.randomize_hints();

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

void build_patched_rom(const std::filesystem::path& input_path, const std::filesystem::path& output_path, 
                       GameData& game_data, RandomizerWorld& world, const RandomizerOptions& options)
{
#ifdef DEBUG
    std::filesystem::remove_all("./tmp_dump/");
#endif

    std::cout << "Checking input image...\n";

    if(!std::filesystem::exists(input_path))
    {
        throw RandomizerException("Input file 'input.bin' is missing from the randomizer folder. "
                                  "Please place your Alundra 1.1 US disc image there and rename it 'input.md'.)");
    }

    size_t file_size = std::filesystem::file_size(input_path);
    if(file_size != 600359760)
    {
        throw RandomizerException("Invalid file size (" + std::to_string(file_size) + ") on the image file. "
                                  "Make sure you are using a 1.1 US image.");
    }

    // Dump the input ROM into a "tmp_dump" folder
    std::cout << "Extracting game files...\n";
    dump_iso(input_path, "./tmp_dump/");

    // Apply patches to relevant files that were extracted from the game ROM
    std::cout << "Editing game files...\n";
    BinaryFile datas_file("./tmp_dump/DATA/DATAS.BIN");
    PsxExeFile exe_file("./tmp_dump/ALUN_CD.EXE");

    apply_randomizer_patches(datas_file, exe_file, game_data, world, options);

    datas_file.save();
    exe_file.save();

    // Use an external tool to repack the files into a PS1 disc image
    std::cout << "Building a disc image...\n";
    if(!rebuild_iso("./tmp_dump/", output_path))
        throw RandomizerException("Could not build disc image. Maybe the file is currently in use?");

#ifndef DEBUG
    std::filesystem::remove_all("./tmp_dump/");
#endif

    std::cout << "Randomized game outputted to " << output_path << ".\n";
}

void generate(const ArgumentDictionary& args)
{
    GameData game_data;
    RandomizerWorld world(game_data);
    RandomizerOptions options(args, game_data, world);
    PersonalSettings personal_settings(args);

    game_data.apply_options(options);
    world.apply_options(options, game_data);

    Json spoiler_json = randomize(world, game_data, options, personal_settings, args);

    // Parse output paths from args
    std::filesystem::path output_rom_path = args.get_string("outputrom", "");
    std::filesystem::path spoiler_log_path = args.get_string("outputlog", "");
    process_paths(output_rom_path, spoiler_log_path, options.hash_sentence());

    // Don't perform the patching process if "only-logic" option was provided
    if(!args.contains("only-logic"))
    {
        std::string input_rom_path = args.get_string("input", "./input.bin");
        build_patched_rom(input_rom_path, output_rom_path, game_data, world, options);
    }
    
    // Write a spoiler log to help the player
    if(!spoiler_log_path.empty())
    {
        std::ofstream spoiler_file(spoiler_log_path);
        if(!spoiler_file)
            throw RandomizerException("Could not open output log file for writing at path '" + spoiler_log_path.string() + "'");

        spoiler_file << spoiler_json.dump(4);
        spoiler_file.close();
        if(options.allow_spoiler_log())
            std::cout << "Spoiler log written into " << spoiler_log_path << ".\n";
        else
            std::cout << "Generation log written into " << spoiler_log_path << ".\n";
    }

    if(args.contains("graph"))
        GraphvizWriter::write_logic_as_dot(world, "./logic.dot");

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
    catch(Json::parse_error& e)
    {
        std::cerr << "ERROR: Malformed json -> " << e.what() << std::endl;
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
