#pragma once

#include "game_patch.hpp"
#include "../tools/byte_array.hpp"
#include "../tools/mips_tools.hpp"

class PatchApplyItemsInfo : public GamePatch
{
public:
    void alter_exe_file(PsxExeFile& exe, const GameData& game_data, const RandomizerWorld& world) override
    {
        constexpr uint32_t ITEM_INFO_TABLE_START = 0x9C650;
        constexpr uint32_t ITEM_INFO_STRUCT_SIZE = 10;

        for(uint8_t item_id = 1 ; item_id < ITEM_COUNT ; ++item_id)
        {
            const Item* item = game_data.item(item_id);
            uint32_t base_addr = ITEM_INFO_TABLE_START + (item_id * ITEM_INFO_STRUCT_SIZE);
            exe.set_word_le(base_addr, item->slot_id());
            exe.set_word_le(base_addr+4, item->tier());
        }
        // Alter the ItemInfoTable to modify wrong tier information
        /*
        exe.set_byte(0x9C6BE, 0x0);  // Remove unused T3 Flail
        exe.set_byte(0x9C6C8, 0x0);  // Remove unused T4 Flail

        exe.set_byte(0x9C69A, 0x0);  // Make Spirit Wand T1 instead of T2

        exe.set_byte(0x9C6E0, 0x0);  // Make Ice Wand T1 instead of T2
        exe.set_byte(0x9C6D2, 0x0);  // Remove unused T1 Ice Wand

        exe.set_byte(0x9C6F4, 0x0);  // Make Fire Wand T1 instead of T2
        exe.set_byte(0x9C6E6, 0x0);  // Remove unused T1 Fire Wand
         */
    }
};
