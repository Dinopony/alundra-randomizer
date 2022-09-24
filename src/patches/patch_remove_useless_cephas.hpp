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
        remove_cephas(data, 0x3664A6);
        remove_cephas(data, 0x3664CE);
        remove_cephas(data, 0x3664E2);
        remove_cephas(data, 0x3664F6);
        remove_cephas(data, 0x36650A);
        remove_cephas(data, 0x36651E);
        remove_cephas(data, 0x3665FA);
        remove_cephas(data, 0x36660E);
        remove_cephas(data, 0x366622);
        remove_cephas(data, 0x366636);
        remove_cephas(data, 0x36664A);
        remove_cephas(data, 0x36665E);
        remove_cephas(data, 0x366672);
        remove_cephas(data, 0x366686);
        remove_cephas(data, 0x366AAA);
        remove_cephas(data, 0x366C8A);
        remove_cephas(data, 0x366CC6);
        remove_cephas(data, 0x366CDA);
        remove_cephas(data, 0x366CEE);
        remove_cephas(data, 0x366D02);

        remove_cephas(data, 0x44591CA);
        remove_cephas(data, 0x4459206);
        remove_cephas(data, 0x49505DA);
        remove_cephas(data, 0x4950602);
        remove_cephas(data, 0x4950616);
        remove_cephas(data, 0x495062A);
        remove_cephas(data, 0x495063E);
        remove_cephas(data, 0x4950652);
        remove_cephas(data, 0x495072E);
        remove_cephas(data, 0x4950742);
        remove_cephas(data, 0x4950756);
        remove_cephas(data, 0x495076A);
        remove_cephas(data, 0x495077E);
        remove_cephas(data, 0x4950792);
        remove_cephas(data, 0x49507A6);
        remove_cephas(data, 0x49507BA);
        remove_cephas(data, 0x4950BDE);
        remove_cephas(data, 0x4950DBE);
        remove_cephas(data, 0x4950DFA);
        remove_cephas(data, 0x4950E0E);
        remove_cephas(data, 0x4950E22);
        remove_cephas(data, 0x4950E36);
    }

private:
    static void remove_cephas(BinaryFile& data, uint32_t entity_base_address)
    {
        // Position Cephas outside of the map
        data.set_word(entity_base_address + 5, 0xFFFF);

        // Remove events linked with that Cephas instance
        data.set_byte(entity_base_address + 10, 0x00);
    }
};

