#pragma once

#include "game_patch.hpp"
#include "../tools/byte_array.hpp"

/**
 * This patch allows changing the amount of deaths required to get King Snow's item (default is 20)
 */
class PatchSetKingSnowDeathCount : public GamePatch
{
private:
    uint8_t _target_death_count;

public:
    explicit PatchSetKingSnowDeathCount(uint8_t target_death_count) : _target_death_count(target_death_count)
    {}

    void alter_datas_file(BinaryFile& data, const GameData& game_data, const RandomizerWorld& world) override
    {
        // Change the amount of deaths required by King Snow to give his item
        data.set_byte(0x29B4BD, _target_death_count);
    }
};
