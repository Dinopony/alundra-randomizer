#pragma once

#include <iostream>
#include "game_patch.hpp"
#include "../model/item_source.hpp"
#include "../game/item.hpp"
#include "../tools/exception.hpp"

/**
 * This patch applies the content of the item sources inside the datas files
 */
class PatchApplyItemSources : public GamePatch
{
public:
    void alter_datas_file(BinaryFile& data, const GameData& game_data, const RandomizerWorld& world) override
    {
        for(ItemSource* source : world.item_sources())
        {
            // Apply item ID addresses
            uint8_t item_id = source->item() ? source->item()->id() : ITEM_NONE;
            for(uint32_t addr : source->addresses())
            {
#ifdef DEBUG
                // In debug builds, check that the data file contains the expected vanilla item ID to avoid any address mistake
                if(source->vanilla_item() && data.get_byte(addr) != source->vanilla_item()->id())
                    std::cerr << "ItemSource '" << source->name() << "' doesn't contain the expected vanilla item" << std::endl;
#endif
                data.set_byte(addr, item_id);
            }

            // Apply sprite ID addresses
            uint8_t item_sprite_id = item_id + 0x1E;
            for(uint32_t addr : source->sprite_addresses())
            {
#ifdef DEBUG
                // In debug builds, check that the data file contains the expected vanilla sprite ID to avoid any address mistake
                if(source->vanilla_item() && data.get_byte(addr) != source->vanilla_item()->id() + 0x1E)
                    std::cerr << "ItemSource '" << source->name() << "' doesn't contain the expected vanilla item" << std::endl;
#endif
                data.set_byte(addr, item_sprite_id);
            }

            // Apply item price addresses
            for(uint32_t addr : source->price_addresses())
            {
                data.set_word_le(addr, source->item()->gold_value());
            }
        }

        // Fix ground Book of Elna byte causing trouble
        data.set_byte(0x1BA5D05, 0x00);

        // Change the "repeating" slot at Merrick's shop visually into a chest
        data.set_byte(0x1EBF9CE, 0x1E);
        data.set_long(0x1EBF9D1, 0x00000000);
    }

    void alter_exe_file(PsxExeFile& exe, const GameData& game_data, const RandomizerWorld& world) override
    {
        constexpr uint32_t ITEM_INFO_TABLE_ADDR = 0x800C7DE4;

        // Handle Merrick shop item table, located inside the code. This is necessary to change the item name.
        for(ItemSource* source : world.item_sources())
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
