#pragma once

#include "game_patch.hpp"
#include "../constants/flags.hpp"

class PatchRemoveCutscenes : public GamePatch {
public:
    void alter_game_data(GameData& game_data) override
    {
        // This skips the need to talk twice to the master sage, given that he pretty much says the same thing
        // when you talk again to validate...
        game_data.add_starting_flag(FLAG_LARS_CRYPT_TALKED_WITH_MASTER_SAGE_ONCE);
    }

    void alter_datas_file(BinaryFile& data, const GameData& game_data, const RandomizerWorld& world) override
    {
        remove_cutscene_after_dark_dragon_fight(data);
        remove_post_coal_mine_cutscenes(data);
        remove_magyscar_entrance_cutscene(data);
        remove_lars_crypt_5_sages_cutscene(data);
        remove_post_wilda_night_cutscene(data);
    }

private:
    static void remove_cutscene_after_dark_dragon_fight(BinaryFile& data)
    {
        // Remove cutscene between Dark Dragon and Melzas 1
        constexpr uint32_t WARP_TO_SANCTUARY_CUTSCENE_ADDR = 0x47C9E15;
        constexpr uint32_t WARP_TO_MELZAS_1_ADDR = 0x67C51C7;
        ByteArray warp_to_melzas_1_instr =  data.get_bytes(WARP_TO_MELZAS_1_ADDR, WARP_TO_MELZAS_1_ADDR + 8);
        data.set_bytes(WARP_TO_SANCTUARY_CUTSCENE_ADDR, warp_to_melzas_1_instr);

        // Get rid of the HUD removal on this map change
        data.set_bytes(0x47C9E12, { 0x02, 0x03, 0x00 }); // Always branch 3 bytes forward
    }

    static void remove_post_coal_mine_cutscenes(BinaryFile& data)
    {
        // Remove "Murggs running in woods" cutscene in Overworld A2
        data.set_bytes(0x13F339, { 0x02, 0x0D, 0x00 }); // Always branch 0xD bytes forward

        // Remove the watchtower cutscene in Overworld A1
        data.set_bytes(0x105DB4, { 0x02, 0x7C, 0x00 }); // Always branch 0x7C bytes forward

        // Remove flag set on meatballs fall so they disappear on map exit + re-enter.
        // This prevents Coal Mine entrance from being blocked, and back exit from being a softlock source if triggered
        // before doing Coal Mine
        data.set_bytes(0x245A04, { 0x02, 0x03, 0x00 });

        // Big rocks disappearing initially requires 2 flags:
        //  - Meatballs fall flag
        //  - Watchtower cutscene flag
        // Remove the meatballs flag requirement for big rocks disappearance
        data.set_byte(0x245A0B, 0x47); // Shorten the branch to not ignore the second branch condition anymore

        // Change the watchtower requirement to a "beat Coal Mine boss" requirement for big rocks disappearance
        data.set_word_le(0x245A50, FLAG_COAL_MINE_BOSS_BEATEN.event_code());

        // Change the watchtower requirement to a "beat Coal Mine boss" requirement for villagers cutscene to trigger
        // in Overworld B2
        data.set_word_le(0x2458A1, FLAG_COAL_MINE_BOSS_BEATEN.event_code());
    }

    /**
     * Sometimes, a cutscene with villagers praying is triggered at Magyscar entrance.
     * This function removes that cutscene.
     */
    static void remove_magyscar_entrance_cutscene(BinaryFile& data)
    {
        // Replace "branch if flag triggering cutscene not set" by "always branch" so the cutscene never triggers
        data.set_bytes(0x2122C20, { 0x02, 0x17, 0x00 });
        data.set_bytes(0x2122C38, { 0x02, 0x2F, 0x00 });
    }

    static void remove_lars_crypt_5_sages_cutscene(BinaryFile& data)
    {
        // Open the door when master sage has been talked to, instead of after seeing the full cutscene
        // This will remove the cutscene where all the sages appear one by one
        data.set_word_le(0x63AC96, FLAG_LARS_CRYPT_SAGES_CUTSCENE_ACTIVE.event_code());
    }

    /**
     * In the vanilla game, defeating Wilda teleports you to a nighttime variant of Inoa where Bergus got kidnapped.
     * We need the bossfight to teleport us to our regular bedroom instead.
     */
    static void remove_post_wilda_night_cutscene(BinaryFile& data)
    {
        // Change the warp destination map
        data.set_word_le(0x4817A36, MAP_JESS_HOUSE);

        // Unfreeze Alundra so he doesn't get stuck in cutscene mode (since he is meant to be unfrozen by another
        // cutscene in nighttime bedroom)
        data.set_byte(0x4817A02, 0x11);
    }
};
