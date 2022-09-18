#pragma once

#include "game_patch.hpp"
#include "../tools/byte_array.hpp"
#include "../constants/flags.hpp"
#include "../randomizer_options.hpp"

class PatchApplyOptionsOnWorld : public GamePatch
{
private:
    RandomizerOptions& _options;

public:
    explicit PatchApplyOptionsOnWorld(RandomizerOptions& options) : _options(options)
    {}

    void alter_world(World& world) override
    {
        if(_options.megaliths_enabled_on_start())
            world.add_starting_flag(FLAG_CAN_ACTIVATE_MEGALITHS);

        if(_options.skip_last_dungeon())
        {
            world.add_starting_flag(FLAG_OPENED_LAKE_SHRINE_EXTERIOR_MAIN_GATE);
            world.add_starting_flag(FLAG_LAKE_SHRINE_EXTERIOR_RIGHT_GARDEN_BUTTON);
            world.add_starting_flag(FLAG_LAKE_SHRINE_EXTERIOR_LEFT_GARDEN_BUTTON);
            world.add_starting_flag(FLAG_LAKE_SHRINE_EXTERIOR_GARDEN_MIDDLE_GATE);
            world.add_starting_flag(FLAG_LAKE_SHRINE_INTERIOR_MELZAS_INTRO_CUTSCENE_1);
            world.add_starting_flag(FLAG_LAKE_SHRINE_INTERIOR_MELZAS_INTRO_CUTSCENE_2);
            world.add_starting_flag(FLAG_LAKE_SHRINE_NW_SEAL_BROKEN);
            world.add_starting_flag(FLAG_LAKE_SHRINE_W_SEAL_BROKEN);
            world.add_starting_flag(FLAG_LAKE_SHRINE_SW_SEAL_BROKEN);
            world.add_starting_flag(FLAG_LAKE_SHRINE_SE_SEAL_BROKEN);
            world.add_starting_flag(FLAG_LAKE_SHRINE_E_SEAL_BROKEN);
            world.add_starting_flag(FLAG_LAKE_SHRINE_NE_SEAL_BROKEN);
        }
    }
};

