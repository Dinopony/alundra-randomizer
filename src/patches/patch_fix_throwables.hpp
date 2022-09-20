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
        constexpr uint32_t OBJECT_PROPERTIES_TABLE_START = 0x800;
        constexpr uint8_t VALID_THROWABLE_ITEM_ID = ITEM_HERBS;
        constexpr uint8_t VALID_THROWABLE_OBJECT_ID = VALID_THROWABLE_ITEM_ID + 0x1E;

        // Read offsets to reach the object properties for an item object that can be thrown properly (e.g. Herbs)
        uint32_t valid_item_offset_addr = OBJECT_PROPERTIES_TABLE_START + (VALID_THROWABLE_OBJECT_ID * 4);
        uint32_t valid_item_offset = data.get_long_le(valid_item_offset_addr);
        uint32_t valid_item_addr = OBJECT_PROPERTIES_TABLE_START + valid_item_offset;

        // Once inside the object properties, go to the first section and extract the first 8 bytes
        uint32_t sub_offset = data.get_long_le(valid_item_addr);
        uint32_t object_infos_addr = valid_item_addr + sub_offset;
        ByteArray valid_bytes = data.get_bytes(object_infos_addr, object_infos_addr + 8);

        // Now, we apply those "valid bytes" to every item object
        for(uint8_t item_id = ITEM_DAGGER ; item_id <= ITEM_CURIOUS_KEY ; ++item_id)
        {
            uint8_t object_id = item_id + 0x1E;
            uint32_t object_addr = OBJECT_PROPERTIES_TABLE_START + (object_id * 4);
            sub_offset = data.get_long_le(object_addr);
            object_infos_addr = object_addr + sub_offset;
            data.set_bytes(object_infos_addr, valid_bytes);
        }
    }
};
