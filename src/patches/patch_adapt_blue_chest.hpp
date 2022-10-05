#pragma once

#include "game_patch.hpp"
#include "../constants/map_codes.hpp"

/**
 * The blue chest in Jess's house is heavily scripted and cannot be used as-is for randomization.
 * This patch adapts the chest to make it actually usable in the randomizer.
 */
class PatchAdaptBlueChest : public GamePatch
{
public:
    void alter_datas_file(BinaryFile& data, const GameData& game_data, const RandomizerWorld& world) override
    {
        // Change opening conditions to having the Small Key, instead of requiring Jess's death
        data.set_word_le(0x3AEA9C3, 0x3140);
        data.set_word_le(0x3AEAB71, 0x3140);

        // Remove music fadeout and music change when opening the chest
        data.set_bytes(0x3AEAB86, { 0x10, 0x10 }); // Immobilize Alundra twice (no effect)
        data.set_bytes(0x3AEAB92, { 0x02, 0x03, 0x0 }); // Jump 3 bytes forward

        // Remove Jess Diary contents reading
        data.set_bytes(0x3AEABB1, { 0x2, 0x3, 0x0 }); // Jump 3 bytes forward

        // Remove Broken Armor announcement, and replace it by a map variant change
        // for Jess house to a variant with the chest opened
        data.set_bytes(0x3AEABB7, { 0x38, 0xF2, 0x00, 0xF2, 0x00 });

        // Get rid of HUD removal on warp
        data.set_bytes(0x3AEABEF, { 0x02, 0x03, 0x0 }); // Jump 3 bytes forward

        ByteArray warp_bytes;
        warp_bytes.add_byte(0x11); // Stop immobilizing Alundra
        // Warp to the same place to reinit the map with a closed chest
        warp_bytes.add_byte(0x53);
        warp_bytes.add_word_le(MAP_JESS_HOUSE_RANDOMIZER);
        warp_bytes.add_bytes({ 0x07, 0x08, 0x01 });
        data.set_bytes(0x3AEABF9, warp_bytes);

        // Remove the chest in the map variant where the chest has been opened
        data.set_word(0x36F3907, 0xFFFF);
    }
};
