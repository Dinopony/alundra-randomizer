#pragma once

#include "game_patch.hpp"
#include "../tools/byte_array.hpp"
#include "../game/room_strings.hpp"
#include "../constants/map_codes.hpp"
#include "../model/item_source.hpp"
#include "../game/item.hpp"
#include "../tools/stringtools.hpp"

/**
 * The game hardcodes lots of text, meaning that some item changes were not reflected inside textboxes when given
 * by NPCs. This patch dynamically changes the incriminated strings to make everything look consistent.
 */
class PatchFixItemNamesInTextboxes : public GamePatch
{
public:
    void alter_datas_file(BinaryFile& data, const GameData& game_data, const RandomizerWorld& world) override
    {
        fix_king_snow_item(data, world);
        fix_post_nirude_items(data, world);
        fix_casino_items(data, world);
    }

private:
    static void fix_king_snow_item(BinaryFile& data, const RandomizerWorld& world)
    {
        RoomStrings strings(MAP_OVERWORLD_B3, data);

        ItemSource* king_snow_source = world.item_source("Overworld B3: King Snow reward for dying a specified amount of times (Legend Sword)");
        stringtools::replace(strings.string(0), "Legendary\\NSword", king_snow_source->item()->name());
        stringtools::replace(strings.string(3), "the Legendary Sword!", king_snow_source->item()->name());

        strings.apply_on_data(data);
    }

    static void fix_post_nirude_items(BinaryFile& data, const RandomizerWorld& world)
    {
        ItemSource* topaz_crest_source = world.item_source("Nirude's Lair: Boss reward chest (Topaz Crest)");
        ItemSource* aqua_cape_source = world.item_source("Nirude's Lair: Boss reward item given by Miming (Aqua Cape)");

        // Fix Nirude exterior rewards after Nirude's Lair
        RoomStrings strings(MAP_OVERWORLD_E1, data);
        stringtools::replace(strings.string(34), "the Topaz Crest", topaz_crest_source->item()->name());
        stringtools::replace(strings.string(36), "the Aqua Cape", aqua_cape_source->item()->name());
        strings.apply_on_data(data);

        // Fix second map variant
        RoomStrings strings_variant(MAP_OVERWORLD_E1_VARIANT, data);
        stringtools::replace(strings_variant.string(34), "the Topaz Crest", topaz_crest_source->item()->name());
        stringtools::replace(strings_variant.string(36), "the Aqua Cape", aqua_cape_source->item()->name());
        strings_variant.apply_on_data(data);
    }

    static void fix_casino_items(BinaryFile& data, const RandomizerWorld& world)
    {
        ItemSource* roulette_1_source = world.item_source("Riverside Casino: 'Wheel of Fortune' chest (Life Vessel)");
        ItemSource* roulette_2_source = world.item_source("Riverside Casino: 'Wheel of Fortune' second reward (Gilded Falcon)");
        ItemSource* shooting_1_source = world.item_source("Riverside Casino: 'Hit Your Mark' chest (Life Vessel)");
        ItemSource* shooting_2_source = world.item_source("Riverside Casino: 'Hit Your Mark' second reward (Gilded Falcon)");
        ItemSource* arena_1_source = world.item_source("Riverside Casino: 'Slime Smash' chest (Life Vessel)");
        ItemSource* arena_2_source = world.item_source("Riverside Casino: 'Slime Smash' second reward (Gilded Falcon)");

        RoomStrings roulette_strings(MAP_CASINO_ROULETTE, data);
        stringtools::replace(roulette_strings.string(44), "Life Vessel", roulette_1_source->item()->name());
        stringtools::replace(roulette_strings.string(24), "a Gilded Falcon", roulette_2_source->item()->name());
        roulette_strings.apply_on_data(data);

        RoomStrings shoot_strings(MAP_CASINO_SHOOTING_RANGE, data);
        stringtools::replace(shoot_strings.string(20), "Life Vessel", shooting_1_source->item()->name());
        stringtools::replace(shoot_strings.string(24), "a Gilded Falcon", shooting_2_source->item()->name());
        shoot_strings.apply_on_data(data);

        RoomStrings arena_strings(MAP_CASINO_SLIME_ARENA, data);
        stringtools::replace(arena_strings.string(80), "Life Vessel", arena_1_source->item()->name());
        stringtools::replace(arena_strings.string(81), "fantastic Gilded\\NFalcon!", arena_2_source->item()->name() + "!\\N");
        stringtools::replace(arena_strings.string(24), "a Gilded Falcon", arena_2_source->item()->name());
        arena_strings.apply_on_data(data);

        RoomStrings bar_strings(MAP_RIVERSIDE_BAR, data);
        bar_strings.string(22) = "\\C'Slime Smash' rewards:\\N- " + arena_1_source->item()->name() + "\\N- " + arena_2_source->item()->name();
        bar_strings.string(23) = "\\C'Hit Your Mark' rewards:\\N- " + shooting_1_source->item()->name() + "\\N- " + shooting_2_source->item()->name();
        bar_strings.string(24) = "\\C'Wheel of Fortune' rewards:\\N- " + roulette_1_source->item()->name() + "\\N- " + roulette_2_source->item()->name();
        bar_strings.apply_on_data(data);
    }
};

