#pragma once

#include "game_patch.hpp"
#include "../tools/byte_array.hpp"
#include "../tools/mips_tools.hpp"
#include "../tools/exception.hpp"

/**
 * This patch removes the following videos from the game disc:
 *  - intro (logos + US opening)
 *  - intro after waiting for title screen (JP opening)
 *  - game ending anime
 */
class PatchRemoveVideos : public GamePatch
{
public:
    explicit PatchRemoveVideos(const RandomizerOptions& options)
    {}

    void alter_exe_file(PsxExeFile& exe, const GameData& game_data, const RandomizerWorld& world) override
    {
        remove_intro_videos();
        remove_ending_video();
        remove_movie_references_in_build_xml();
    }

private:
    static void remove_intro_videos()
    {
        BinaryFile boot_exe_file("./tmp_dump/SLUS_005.53");

        // 5DCC - 5E08 filled with nops --> No opening videos played
        for(uint32_t addr = 0x5DDC ; addr < 0x5E08 ; addr += 0x4)
            boot_exe_file.set_long(addr, 0x0);

        // 5A90 - 5AC0 filled with nops --> No opening videos loaded, removing files from disk is possible
        for(uint32_t addr = 0x5A90 ; addr < 0x5AC0 ; addr += 0x4)
            boot_exe_file.set_long(addr, 0x0);

        boot_exe_file.save();
    }

    static void remove_ending_video()
    {
        BinaryFile end_exe_file("./tmp_dump/END.EXE");

        // 1B0C - 1B24 filled with nops --> No ending video played
        for(uint32_t addr = 0x1B0C ; addr < 0x1B24 ; addr += 0x4)
            end_exe_file.set_long(addr, 0x0);

        // 1AC4 - 1AD8 filled with nops --> No ending video loaded, removing file from disk is possible
        for(uint32_t addr = 0x1AC4 ; addr < 0x1AD8 ; addr += 0x4)
            end_exe_file.set_long(addr, 0x0);

        end_exe_file.save();
    }

    static void remove_movie_references_in_build_xml()
    {
        std::ifstream build_file("./tmp_dump/build.xml");
        if(!build_file.is_open())
            throw RandomizerException("Cannot open build file for read");

        std::vector<std::string> file_lines;
        std::string str;
        while (std::getline(build_file, str))
        {
            if(str.find("MATRIX.MOV") != std::string::npos)
                continue;
            if(str.find("USA_OP.MOV") != std::string::npos)
                continue;
            if(str.find("ARAN_OP.MOV") != std::string::npos)
                continue;
            if(str.find("ARAN_END.MOV") != std::string::npos)
                continue;

            file_lines.emplace_back(str);
        }
        build_file.close();

        std::filesystem::remove("./tmp_dump/build.xml");

        std::ofstream build_file_out("./tmp_dump/build.xml");
        if(!build_file_out.is_open())
            throw RandomizerException("Cannot open build file for write");

        for(const std::string& line : file_lines)
            build_file_out << line << "\n";
        build_file_out.close();
    }
};
