#pragma once

#include "game_patch.hpp"
#include "../tools/byte_array.hpp"

/**
 * In vanilla game, there are many different Cephas instances inside the graveyard that apeear on different flag
 * conditions. In randomizer, since the game is non-linear, we can have many Cephas appearing at once, causing glitchy
 * dialogues and even sometimes blocking the graveyard entrance.
 * This patch removes all the Cephas instances but one, that gives the hint to his house's basement riddle.
 */
class PatchRemoveUselessCephas : public GamePatch
{
public:
    void alter_datas_file(BinaryFile& data, const GameData& game_data, const RandomizerWorld& world) override
    {
        const std::vector<uint8_t> ALWAYS_BRANCH = { 0x31, 0x00, 0x00 };

        // Base variant
//        data.set_bytes(0x364B75, ALWAYS_BRANCH);      // Flag D0
        data.set_bytes(0x364B7D, ALWAYS_BRANCH);      // Flag D1
        data.set_bytes(0x364B85, ALWAYS_BRANCH);      // Flag D2
        data.set_bytes(0x364B8D, ALWAYS_BRANCH);      // Flag D3
        data.set_bytes(0x364B95, ALWAYS_BRANCH);      // Flag D4
        data.set_bytes(0x364B9D, ALWAYS_BRANCH);      // Flag D5
        data.set_bytes(0x364BA5, ALWAYS_BRANCH);      // Flag D6
        data.set_bytes(0x364BAD, ALWAYS_BRANCH);      // Flag D7
        data.set_bytes(0x364BCE, ALWAYS_BRANCH);      // Flag D9
        data.set_bytes(0x364BD6, ALWAYS_BRANCH);      // Flag DA
        data.set_bytes(0x364BDE, ALWAYS_BRANCH);      // Flag DB
        data.set_bytes(0x364BE6, ALWAYS_BRANCH);      // Flag DC
        data.set_bytes(0x364BEE, ALWAYS_BRANCH);      // Flag DD

        // Other variant
//        data.set_bytes(0x494ECA9, ALWAYS_BRANCH);      // Flag D0
        data.set_bytes(0x494ECB1, ALWAYS_BRANCH);      // Flag D1
        data.set_bytes(0x494ECB9, ALWAYS_BRANCH);      // Flag D2
        data.set_bytes(0x494ECC1, ALWAYS_BRANCH);      // Flag D3
        data.set_bytes(0x494ECC9, ALWAYS_BRANCH);      // Flag D4
        data.set_bytes(0x494ECD1, ALWAYS_BRANCH);      // Flag D5
        data.set_bytes(0x494ECD9, ALWAYS_BRANCH);      // Flag D6
        data.set_bytes(0x494ECE1, ALWAYS_BRANCH);      // Flag D7
        data.set_bytes(0x494ED02, ALWAYS_BRANCH);      // Flag D9
        data.set_bytes(0x494ED0A, ALWAYS_BRANCH);      // Flag DA
        data.set_bytes(0x494ED12, ALWAYS_BRANCH);      // Flag DB
        data.set_bytes(0x494ED1A, ALWAYS_BRANCH);      // Flag DC
        data.set_bytes(0x494ED22, ALWAYS_BRANCH);      // Flag DD
    }
};

