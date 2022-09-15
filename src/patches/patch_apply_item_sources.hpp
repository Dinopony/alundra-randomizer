#pragma once

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
            if(source->is_ground_item())
                item_id += 0x1E;

            for(uint32_t addr : source->addresses())
                data.set_byte(addr, item_id);
        }
    }
};
