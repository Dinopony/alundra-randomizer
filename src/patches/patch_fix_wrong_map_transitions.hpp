#pragma once

#include "game_patch.hpp"
#include "../tools/byte_array.hpp"

class PatchFixWrongMapTransitions : public GamePatch
{
public:
    void alter_datas_file(BinaryFile& data, const GameData& game_data, const RandomizerWorld& world) override
    {
        const std::vector<uint8_t> NEVER_BRANCH = { 0x30, 0x00, 0x00, 0x00, 0x05 };

        // Leaving the inn map uses a forced transition to the wrong variant of Inoa, replace
        // the map ID directly in the events
        data.set_bytes(0x3A6729E, NEVER_BRANCH);
        data.set_word_le(0x3A672A4, 0x00F1);

        // There is an event in Lutas's house second floor that overwrites the map variant for the ground floor,
        // in order to ensure the stairs will bring us to the right map. The problem is, this relies on story flags
        // and is a pain for us, so we just force the variant to be the one that is good for us.
        data.set_bytes(0x4996860, NEVER_BRANCH);
        data.set_word_le(0x4996868, 0x00F5);
    }
};
