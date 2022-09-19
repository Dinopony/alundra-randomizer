#include "patches.hpp"

#include "../randomizer_options.hpp"

#include "patch_new_game.hpp"
#include "patch_fix_map_inconsistencies.hpp"
#include "patch_apply_item_sources.hpp"
#include "patch_original_game_balance.hpp"
#include "patch_neutralize_map_variant_changes.hpp"
#include "patch_change_flag_checks.hpp"
#include "patch_allow_using_consumables_while_full.hpp"
#include "patch_apply_options_on_world.hpp"

void execute_patches(const std::vector<GamePatch*>& patches, BinaryFile& data, PsxExeFile& exe, World& world)
{
    for(GamePatch* patch : patches) patch->alter_world(world);
    for(GamePatch* patch : patches) patch->alter_data(data, world);
    for(GamePatch* patch : patches) patch->alter_exe(exe, world);
    for(GamePatch* patch : patches) delete patch;
}

void apply_randomizer_patches(BinaryFile& data, PsxExeFile& exe, World& world, const RandomizerOptions& options)
{
    std::vector<GamePatch*> patches;

    patches.emplace_back(new PatchApplyOptionsOnWorld(options));
    patches.emplace_back(new PatchNewGame());
    patches.emplace_back(new PatchFixMapInconsistencies());
    patches.emplace_back(new PatchApplyItemSources());
    patches.emplace_back(new PatchNeutralizeMapVariantChanges());
    patches.emplace_back(new PatchChangeFlagChecks());
    patches.emplace_back(new PatchAllowUsingConsumablesWhileFull());
    if(options.original_game_balance())
        patches.emplace_back(new PatchOriginalGameBalance());

    execute_patches(patches, data, exe, world);
}
