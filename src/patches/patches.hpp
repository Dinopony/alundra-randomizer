#pragma once

#include "game_patch.hpp"
#include "../model/world.hpp"

#include "patch_new_game.hpp"
#include "patch_fix_map_inconsistencies.hpp"
#include "patch_apply_item_sources.hpp"

void execute_patches(const std::vector<GamePatch*>& patches, BinaryFile& data, PsxExeFile& exe, World& world)
{
    for(GamePatch* patch : patches) patch->alter_world(world);
    for(GamePatch* patch : patches) patch->alter_data(data, world);
    for(GamePatch* patch : patches) patch->alter_exe(exe);
    for(GamePatch* patch : patches) delete patch;
}

void apply_randomizer_patches(BinaryFile& data, PsxExeFile& exe, World& world, RandomizerOptions& options)
{
    std::vector<GamePatch*> patches;

    patches.emplace_back(new PatchNewGame(options.megaliths_enabled_on_start()));
    patches.emplace_back(new PatchFixMapInconsistencies());
    patches.emplace_back(new PatchApplyItemSources());

    execute_patches(patches, data, exe, world);
}