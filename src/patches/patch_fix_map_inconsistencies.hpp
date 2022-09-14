#pragma once

#include "game_patch.hpp"
#include "../tools/byte_array.hpp"

class PatchFixMapInconsistencies : public GamePatch
{
public:
    void alter_data(BinaryFile& data, const World& world) override
    {
        // Map 407 (Overworld C4 with Merrick's shop open) has two bugged chests having different chest placement
        // & contents compared to map 12 (Overworld C4 with Merrick's shop closed).
        // This patch changes map 407 so it follows map 12 which becomes the standard
        // Don't change the chest contents, since the ItemSource will handle synchronizing both values

        // Left coast chest
        // Map 12:  01 40 1E 0D 4E 06 00 00 00 00 00 00 D6 06 26
        // Map 407: 01 40 1E 0C 4E 06 00 00 00 00 00 00 40 05 29
        data.set_byte(0x584D6BF, 0x0D);
        data.set_word(0x584D6C8, 0xD606);

        // Right chest in the forest
        // Map 12:  01 40 1E 5F 3E 0C 00 00 00 00 00 00 D7 06 25
        // Map 407: 01 40 1E 5E 3E 0C 00 00 00 00 00 00 42 05 26
        data.set_byte(0x584D6AB, 0x5F);
        data.set_word(0x584D6B4, 0xD706);
    }
};
