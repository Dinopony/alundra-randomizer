#pragma once

#include "game_patch.hpp"
#include "../tools/byte_array.hpp"

class PatchNeutralizeMapVariantChanges : public GamePatch
{
public:
    void alter_data(BinaryFile& data, const World& world) override
    {
        constexpr uint8_t INSTRUCTION_CODE_DO_NOTHING = 0xC5;

        // Neutralize the Inoa variant replacement in the Murgg watchtower cutscene
        data.set_byte(0x105E29, INSTRUCTION_CODE_DO_NOTHING);
    }

    void alter_exe(PsxExeFile& exe) override
    {
        // Add a "do nothing" event instruction to replace unconvenient instructions (with code 0xC5)
        MipsCode func;
        func.jr(reg_RA, false);
        func.li(reg_V0, 0x5);
        uint32_t func_addr = exe.inject_code(func);
        exe.set_long_le(0x7C328, func_addr);
    }
};
