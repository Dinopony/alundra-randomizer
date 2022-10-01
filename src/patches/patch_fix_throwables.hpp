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
    static uint32_t get_object_properties_addr(BinaryFile& data, uint8_t item_id)
    {
        constexpr uint32_t OBJECT_PROPERTIES_TABLE_START = 0x800;

        uint8_t item_object_id = item_id + 0x1E;
        uint32_t offset_1 = data.get_long_le(OBJECT_PROPERTIES_TABLE_START + (item_object_id * 4));
        uint32_t offset_2 = data.get_long_le(OBJECT_PROPERTIES_TABLE_START + offset_1);
        return OBJECT_PROPERTIES_TABLE_START + offset_2;
    }


    void alter_datas_file(BinaryFile& data, const GameData& game_data, const RandomizerWorld& world) override
    {
        // Prevent Merrick items from being taken and thrown to prevent any crash
        data.set_byte(0x1EBF9E8, 0x00);
        data.set_byte(0x1EBF9FC, 0x00);
        data.set_byte(0x1EBFA10, 0x00);

        // Get pointer offsets for valid item (Life Vessel)
        constexpr uint32_t STARTING_ADDR = 0x830;
        uint32_t life_vessel_offset_addr = STARTING_ADDR + ((ITEM_LIFE_VESSEL + 0x1E) * 4);
        uint32_t life_vessel_offset = data.get_long_le(life_vessel_offset_addr);
        uint32_t life_veseel_ptr_table = 0x800 + life_vessel_offset;
        uint32_t life_vessel_ptr_1 = data.get_long(life_veseel_ptr_table); // 5C300300
        uint32_t life_vessel_ptr_2 = data.get_long(life_veseel_ptr_table + 4); // A4300300

        for(uint16_t item_id = 0x1 ; item_id < ITEM_COUNT ; ++item_id)
        {
            uint32_t addr = STARTING_ADDR + ((item_id + 0x1E) * 4);

            uint32_t pointer_table_offset = data.get_long_le(addr) + 0x800;
            data.set_long(pointer_table_offset, life_vessel_ptr_1);
            data.set_long(pointer_table_offset + 4, life_vessel_ptr_2);
        }
    }
};
