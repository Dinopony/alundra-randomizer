#pragma once

#include "game_patch.hpp"
#include "../tools/byte_array.hpp"
#include "../constants/item_codes.hpp"

/**
 * In the vanilla game, only a handful of items can be found in shops and be taken inside Alundra's hands.
 * Developers only set valid physics properties to item objects that could be encountered inside shops,
 * not to every possible item object. This means, if we want any item to work inside shops, we need to copy
 * the "good" physics properties from a well-handled object to all other item objects.
 * This patch fixes that bug by doing exactly this.
 */
class PatchFixThrowables : public GamePatch
{
public:
    void alter_datas_file(BinaryFile& data, const GameData& game_data, const RandomizerWorld& world) override
    {
        // Prevent Merrick items from being taken and thrown to prevent any crash
        data.set_byte(0x1EBF9E8, 0x00);
        data.set_byte(0x1EBF9FC, 0x00);
        data.set_byte(0x1EBFA10, 0x00);

        // Get pointer offsets for valid item (Life Vessel)
        constexpr uint32_t STARTING_ADDR = 0x8a8;
        uint32_t life_vessel_offset_addr = STARTING_ADDR + (ITEM_LIFE_VESSEL * 4);
        uint32_t life_vessel_offset = data.get_long_le(life_vessel_offset_addr);
        uint32_t life_vessel_ptr_table = 0x800 + life_vessel_offset;
        uint32_t life_vessel_ptr_1 = data.get_long(life_vessel_ptr_table); // 5C300300
        uint32_t life_vessel_ptr_2 = data.get_long(life_vessel_ptr_table + 4); // A4300300

        for(uint16_t item_id = 0x1 ; item_id < ITEM_COUNT ; ++item_id)
        {
            uint32_t ptr_addr = STARTING_ADDR + (item_id * 4);
            uint32_t addr = 0x800 + data.get_long_le(ptr_addr);
            // Add shadows and give the same properties to all item entities
            data.set_word(addr + 0x18, 0xF6F9);
            data.set_word(addr + 0x1B, 0x150F);
            // Prevent crashes when throwing items
            data.set_long(addr, life_vessel_ptr_1);
            data.set_long(addr + 4, life_vessel_ptr_2);
        }
    }
};
