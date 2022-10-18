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

        constexpr uint32_t ITEM_INFO_2_TABLE_START = 0xA85E4;
        constexpr uint32_t ITEM_INFO_2_STRUCT_SIZE = 8;

        // Prepare an array with booleans indicating whether item is owned on game start
        std::vector<bool> items_owned_on_start;
        items_owned_on_start.resize(ITEM_COUNT, false);
        for(const Item* item : world.starting_inventory())
            items_owned_on_start[item->id()] = true;

        // For each item, edit its info on both ItemInfo tables
        for(uint8_t item_id = 1 ; item_id < ITEM_COUNT ; ++item_id)
        {
            const Item* item = game_data.item(item_id);

            uint32_t item_info_1_addr = ITEM_INFO_TABLE_START + (item_id * ITEM_INFO_STRUCT_SIZE);
            uint32_t item_info_2_addr = ITEM_INFO_2_TABLE_START + (item_id * ITEM_INFO_2_STRUCT_SIZE);

            // Set inventory slot used by this item
            exe.set_word_le(item_info_1_addr, item->slot_id());

            // Set tier of this item for the given slot
            exe.set_word_le(item_info_1_addr + 4, item->tier());

            // Set max carriable quantity for the item
            exe.set_word_le(item_info_1_addr + 6, item->max_quantity()); // First table carries the "functional max qty"
            exe.set_byte(item_info_2_addr + 7, item->max_quantity());

            // If item was using the fanfare sound effect (0x00), use the common item sound effect instead (0x34)
            if(exe.get_byte(item_info_2_addr + 5) == 0x00)
                exe.set_byte(item_info_2_addr + 5, 0x34);

            // Set a specific flag if item is inside starting inventory
            uint8_t flag = exe.get_byte(item_info_2_addr + 6) & 0x7F;
            if(items_owned_on_start[item_id])
                flag |= 0x80;
            exe.set_byte(item_info_2_addr + 6, flag);
        }
    }
};
