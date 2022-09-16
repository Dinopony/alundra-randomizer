#include "patches.hpp"

#include "../randomizer_options.hpp"

#include "patch_new_game.hpp"
#include "patch_fix_map_inconsistencies.hpp"
#include "patch_apply_item_sources.hpp"
#include "patch_original_game_balance.hpp"
#include "patch_neutralize_map_variant_changes.hpp"

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
    patches.emplace_back(new PatchNeutralizeMapVariantChanges());
    if(options.original_game_balance())
        patches.emplace_back(new PatchOriginalGameBalance());

    execute_patches(patches, data, exe, world);
}
