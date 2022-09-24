#pragma once

#include "game_patch.hpp"
#include "../tools/byte_array.hpp"

class PatchNeutralizeFlags : public GamePatch
{
public:
    void alter_datas_file(BinaryFile& data, const GameData& game_data, const RandomizerWorld& world) override
    {
        constexpr uint8_t INSTRUCTION_CODE_SKIP_FLAG_SET = 0xC6;

        // Remove the flag set when entering Lars Crypt that prevents from re-entering
        data.set_byte(0x3654C4, INSTRUCTION_CODE_SKIP_FLAG_SET);
        data.set_byte(0x494F5F8, INSTRUCTION_CODE_SKIP_FLAG_SET);

        // Remove Alundra being immobilized when seeing Magyscar cutscene to prevent a softlock
        data.set_byte(0x1683D14, 0x11);
    }

    void alter_exe_file(PsxExeFile& exe, const GameData& game_data, const RandomizerWorld& world) override
    {
        // Add a "do nothing" event instruction to replace unconvenient instructions (with code 0xC6)
         MipsCode func;
         func.jr(reg_RA, false);
         func.li(reg_V0, 0x3);
         uint32_t func_addr = exe.inject_code(func);
         exe.set_long_le(0x7C32C, func_addr);
    }
};
