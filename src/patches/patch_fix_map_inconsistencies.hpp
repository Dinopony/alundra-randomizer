#pragma once

#include "game_patch.hpp"
#include "../tools/byte_array.hpp"
#include "../constants/flags.hpp"

class PatchFixMapInconsistencies : public GamePatch
{
public:
    void alter_datas_file(BinaryFile& data, const GameData& game_data, const RandomizerWorld& world) override
    {
        uniformize_overworld_c4_variants(data);
        split_flags_between_ancient_shrine_and_magyscar(data);
    }

private:
    /**
     * Map 407 (Overworld C4 with Merrick's shop open) has two bugged chests having different chest placement
     * & contents compared to map 12 (Overworld C4 with Merrick's shop closed).
     * This patch changes map 407 so it follows map 12 which becomes the standard.
     * Don't change the chest contents, since the ItemSource will handle synchronizing both values
     */
    static void uniformize_overworld_c4_variants(BinaryFile& data)
    {
        // Left coast chest
        // Map 12:  01 40 1E 0D 4E 06 00 00 00 00 00 00 D6 06 26
        // Map 407: 01 40 1E 0C 4E 06 00 00 00 00 00 00 40 05 29
        data.set_byte(0x584D6BF, 0x0D);
        data.set_word(0x584D6C8, 0xD606);
        data.set_byte(0x584D6CA, 0x26);

        // Right chest in the forest
        // Map 12:  01 40 1E 5F 3E 0C 00 00 00 00 00 00 D7 06 25
        // Map 407: 01 40 1E 5E 3E 0C 00 00 00 00 00 00 42 05 26
        data.set_byte(0x584D6AB, 0x5F);
        data.set_word(0x584D6B4, 0xD706);
        data.set_byte(0x584D6B6, 0x25);
    }

    /**
     * Flag 0x1DD3F8, bit 7 is set under any of the two conditions:
     * - the "Symbols pillar room" in Ancient Shrine is cleared (empties the room when coming back)
     * - a bush not far from Magyscar entrance leading to a room with a Life Vessel (removes the bush when coming back)
     *
     * An unknown mechanism must clear this flag at some point in the vanilla game to be able to re-use it, but this
     * doesn't happen during a randomizer playthrough. This patch changes the flag used in Magyscar to make it different
     * from the one in Ancient Shrine.
     */
    static void split_flags_between_ancient_shrine_and_magyscar(BinaryFile& data)
    {
        // Flag set in Magyscar
        data.set_word_le(0x14E1FAE, FLAG_JESS_TALKED_LEAVING_HOUSE_ON_GAME_START.event_code());

        // Flag check in Magyscar
        data.set_word_le(0x14E1FA1, FLAG_JESS_TALKED_LEAVING_HOUSE_ON_GAME_START.event_code());
    }
};
