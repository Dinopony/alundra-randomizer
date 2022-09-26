#include "patches.hpp"

#include "../randomizer_options.hpp"

#include "patch_new_game.hpp"
#include "patch_fix_map_inconsistencies.hpp"
#include "patch_apply_item_sources.hpp"
#include "patch_original_game_balance.hpp"
#include "patch_neutralize_map_variant_changes.hpp"
#include "patch_neutralize_flags.hpp"
#include "patch_change_flag_checks.hpp"
#include "patch_allow_using_consumables_while_full.hpp"
#include "patch_remove_useless_cephas.hpp"
#include "patch_remove_cutscenes.hpp"
#include "patch_set_king_snow_death_count.hpp"
#include "patch_remove_items_fanfare.hpp"
#include "patch_fix_wrong_map_transitions.hpp"
#include "patch_fix_item_names_in_textboxes.hpp"
#include "patch_fix_throwables.hpp"
#include "patch_split_boots_effects.hpp"

void execute_patches(const std::vector<GamePatch*>& patches, 
                     BinaryFile& data_file, PsxExeFile& exe_file, 
                     GameData& game_data, RandomizerWorld& world)
{
    for(GamePatch* patch : patches) patch->alter_game_data(game_data);
    for(GamePatch* patch : patches) patch->alter_datas_file(data_file, game_data, world);
    for(GamePatch* patch : patches) patch->alter_exe_file(exe_file, game_data, world);
    for(GamePatch* patch : patches) delete patch;
}

void apply_randomizer_patches(BinaryFile& data, PsxExeFile& exe, 
                              GameData& game_data, RandomizerWorld& world,
                              const RandomizerOptions& options)
{
    std::vector<GamePatch*> patches;

    patches.emplace_back(new PatchNewGame());
    patches.emplace_back(new PatchFixMapInconsistencies());
    patches.emplace_back(new PatchFixWrongMapTransitions());
    patches.emplace_back(new PatchApplyItemSources());
    patches.emplace_back(new PatchNeutralizeMapVariantChanges());
    patches.emplace_back(new PatchNeutralizeFlags());
    patches.emplace_back(new PatchChangeFlagChecks());
    patches.emplace_back(new PatchAllowUsingConsumablesWhileFull());
    patches.emplace_back(new PatchRemoveUselessCephas());
    patches.emplace_back(new PatchRemoveCutscenes());
    patches.emplace_back(new PatchSetKingSnowDeathCount(options.king_snow_death_count()));
    patches.emplace_back(new PatchRemoveItemsFanfare());
    patches.emplace_back(new PatchFixItemNamesInTextboxes());
    patches.emplace_back(new PatchFixThrowables());
    patches.emplace_back(new PatchSplitBootsEffects());

    if(options.original_game_balance())
        patches.emplace_back(new PatchOriginalGameBalance());

    execute_patches(patches, data, exe, game_data, world);
}
