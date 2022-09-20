#pragma once

#include "game_patch.hpp"
#include "../tools/byte_array.hpp"

class PatchNeutralizeMapVariantChanges : public GamePatch
{
public:
    void alter_datas_file(BinaryFile& data, const GameData& game_data, const RandomizerWorld& world) override
    {
        constexpr uint8_t INSTRUCTION_CODE_SKIP_MAP_VARIANT_SET = 0xC5;

        // Neutralize the Inoa variant replacement in the Murgg watchtower cutscene
        data.set_byte(0x105E29, INSTRUCTION_CODE_SKIP_MAP_VARIANT_SET);

        // Neutralize the Inoa variant replacement when beating Ancient Shrine boss
        data.set_byte(0x82E6DC, INSTRUCTION_CODE_SKIP_MAP_VARIANT_SET);

        // Neutralize the Inoa variant replacement when clearing Coastal Cave
        data.set_byte(0x127B08A, INSTRUCTION_CODE_SKIP_MAP_VARIANT_SET);

        // Neutralize the Inoa variant replacement when clearing Tower of the Great Tree
        data.set_byte(0x22EAD1F, INSTRUCTION_CODE_SKIP_MAP_VARIANT_SET);
    }

    void alter_exe_file(PsxExeFile& exe, const GameData& game_data, const RandomizerWorld& world) override
    {
        // Add a "do nothing" event instruction to replace unconvenient instructions (with code 0xC5)
        MipsCode func;
        func.jr(reg_RA, false);
        func.li(reg_V0, 0x5);
        uint32_t func_addr = exe.inject_code(func);
        exe.set_long_le(0x7C328, func_addr);
    }
};
