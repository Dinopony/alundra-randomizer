#pragma once

#include "game_patch.hpp"
#include "../tools/byte_array.hpp"
#include "../game/room_strings.hpp"
#include "../constants/map_codes.hpp"

/**
 * In vanilla game, there are many different Nava instances inside his house, blocking the way to Nava's Keep.
 * In randomizer, we want to be able to access this dungeon at any time, so we just remove him from existence
 * altogether (at least, in his cabin!)
 */
class PatchRemoveNavaInstances : public GamePatch
{
public:
    void alter_datas_file(BinaryFile& data, const GameData& game_data, const RandomizerWorld& world) override
    {
        const std::vector<uint8_t> ALWAYS_BRANCH = { 0x31, 0x00, 0x00 };

        data.set_bytes(0x20EA7E1, ALWAYS_BRANCH);  // Flag CA
        data.set_bytes(0x20EA7EF, ALWAYS_BRANCH);  // Flag CB
        data.set_bytes(0x20EA7FD, ALWAYS_BRANCH);  // Flag CC
        data.set_bytes(0x20EA80B, ALWAYS_BRANCH);  // Flag CD
        data.set_bytes(0x20EA819, ALWAYS_BRANCH);  // Flag CE
        data.set_bytes(0x20EA827, ALWAYS_BRANCH);  // Flag CF
        data.set_bytes(0x20EA835, ALWAYS_BRANCH);  // Flag D0
        data.set_bytes(0x20EA843, ALWAYS_BRANCH);  // Flag D1
        data.set_bytes(0x20EA851, ALWAYS_BRANCH);  // Flag D2
        data.set_bytes(0x20EA85F, ALWAYS_BRANCH);  // Flag D3
        data.set_bytes(0x20EA86D, ALWAYS_BRANCH);  // Flag D4
        data.set_bytes(0x20EA87B, ALWAYS_BRANCH);  // Flag D5
        data.set_bytes(0x20EA889, ALWAYS_BRANCH);  // Flag D6
        data.set_bytes(0x20EA897, ALWAYS_BRANCH);  // Flag D7
        data.set_bytes(0x20EA8A5, ALWAYS_BRANCH);  // Flag D8
        data.set_bytes(0x20EA8B3, ALWAYS_BRANCH);  // Flag D9
        data.set_bytes(0x20EA8C1, ALWAYS_BRANCH);  // Flag DA
        data.set_bytes(0x20EA8CF, ALWAYS_BRANCH);  // Flag DB
        data.set_bytes(0x20EA8DD, ALWAYS_BRANCH);  // Flag DC
        data.set_bytes(0x20EA8EB, ALWAYS_BRANCH);  // Flag DD
        data.set_bytes(0x20EA8F9, ALWAYS_BRANCH);  // Flag DE
        data.set_bytes(0x20EA8FF, ALWAYS_BRANCH);  // Flag DF
        data.set_bytes(0x20EA90D, ALWAYS_BRANCH);  // Flag E0
    }
};

