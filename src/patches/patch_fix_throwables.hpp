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

        // TODO: This patch doesn't work for now, so we just prevent Merrick items from being thrown
        /*
        constexpr uint8_t VALID_THROWABLE_ITEM_ID = ITEM_HERBS;

        // Extract the first 8 bytes of object properties for a valid item object
        uint32_t valid_bytes_addr = get_object_properties_addr(data, VALID_THROWABLE_ITEM_ID);
        ByteArray valid_bytes = data.get_bytes(valid_bytes_addr, valid_bytes_addr + 8);

        // Now, we apply those "valid bytes" to every item object
        for(uint8_t item_id = ITEM_DAGGER ; item_id <= ITEM_CURIOUS_KEY ; ++item_id)
        {
            uint32_t bytes_addr = get_object_properties_addr(data, item_id);
            data.set_bytes(bytes_addr, valid_bytes);
        }
        */
    }
};
