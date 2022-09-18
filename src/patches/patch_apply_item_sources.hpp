#pragma once

#include <iostream>
#include "game_patch.hpp"
#include "../model/randomizer_world.hpp"
#include "../model/item_source.hpp"
#include "../model/item.hpp"

/**
 * This patch applies the content of the item sources inside the datas files
 */
class PatchApplyItemSources : public GamePatch
{
public:
    void alter_data(BinaryFile& data, const World& world) override
    {
        const RandomizerWorld& randomizer_world = reinterpret_cast<const RandomizerWorld&>(world);
        for(ItemSource* source : randomizer_world.item_sources())
        {
            uint8_t item_id = source->item() ? source->item()->id() : ITEM_NONE;
            for(uint32_t addr : source->addresses())
                data.set_byte(addr, item_id);

            uint8_t item_sprite_id = item_id + 0x1E;
            for(uint32_t addr : source->sprite_addresses())
                data.set_byte(addr, item_sprite_id);
        }

        // Fix ground Book of Elna byte causing trouble
        data.set_byte(0x1BA5D05, 0x00);

        // Change the "repeating" slot at Merrick's shop visually into a chest
        data.set_byte(0x1EBF9CE, 0x1E);
        data.set_long(0x1EBF9D1, 0x00000000);
    }

    void alter_exe(PsxExeFile& exe, const World& world) override
    {
        constexpr uint32_t ITEM_INFO_TABLE_ADDR = 0x800C7DE4;
        const RandomizerWorld& randomizer_world = reinterpret_cast<const RandomizerWorld&>(world);

        // Handle Merrick shop item table, located inside the code. This is necessary to change the item name.
        for(ItemSource* source : randomizer_world.item_sources())
        {
            uint32_t merrick_item_address = source->merrick_item_address();
            if(merrick_item_address)
            {
                uint8_t item_id = source->item() ? source->item()->id() : ITEM_NONE;
                exe.set_long_le(merrick_item_address, ITEM_INFO_TABLE_ADDR + 8 * item_id);
            }
        }
    }
};
