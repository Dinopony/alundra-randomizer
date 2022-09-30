#pragma once

#pragma once

#include "game_patch.hpp"
#include "../tools/byte_array.hpp"
#include "../tools/mips_tools.hpp"
#include "../tools/exception.hpp"

class PatchEditCredits : public GamePatch
{
public:
    void alter_exe_file(PsxExeFile& exe, const GameData& game_data, const RandomizerWorld& world) override
    {
        BinaryFile credits_exe_file("./tmp_dump/CLOSING.EXE");

        for(uint32_t addr = 0xBB0 ; addr < 0xBFA ; ++addr)
            credits_exe_file.set_byte(addr, 0x0);
        for(uint32_t addr = 0x1044C ; addr < 0x10474 ; ++addr)
            credits_exe_file.set_byte(addr, 0x0);

        std::string str_rando = "RANDOMIZER ";
        credits_exe_file.set_bytes(0xD04, reinterpret_cast<const uint8_t*>(str_rando.c_str()), str_rando.size());
        credits_exe_file.set_long_le(0x10440, 0xD04 + 0x8001F800);

        std::string str_author = "Dinopony";
        credits_exe_file.set_bytes(0xBB0, reinterpret_cast<const uint8_t*>(str_author.c_str()), str_author.size());
        credits_exe_file.set_long_le(0x10448, 0xBB0 + 0x8001F800);

        credits_exe_file.save();
    }
};
