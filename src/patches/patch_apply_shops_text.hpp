#pragma once

#include "game_patch.hpp"
#include "../tools/byte_array.hpp"
#include "../game/room_strings.hpp"
#include "../constants/map_codes.hpp"
#include "../tools/stringtools.hpp"
#include "../tools/textbox_formatter.hpp"

class PatchApplyShopsText : public GamePatch
{
public:
    void alter_datas_file(BinaryFile& data, const GameData& game_data, const RandomizerWorld& world) override
    {
        apply_nadia_shop_text(data, world);
        apply_lurvy_shop_text(data, world);
    }

private:
    static void apply_lurvy_shop_text(BinaryFile& data, const RandomizerWorld& world)
    {
        ItemSource* lurvy_1 = world.item_source("Lurvy's Shop: Item #1 (Herbs)");
        ItemSource* lurvy_2 = world.item_source("Lurvy's Shop: Item #2 (Strength Tonic)");
        ItemSource* lurvy_3 = world.item_source("Lurvy's Shop: Item #3 (Strength Elixyr)");
        ItemSource* lurvy_4 = world.item_source("Lurvy's Shop: Item #4 (Magic Elixyr)");
        ItemSource* lurvy_5_1 = world.item_source("Lurvy's Shop: Item #5 (Leather Armor)");
        ItemSource* lurvy_5_2 = world.item_source("Lurvy's Shop: Item #5 replacement when bought once (Life Vessel)");
        ItemSource* lurvy_5_3 = world.item_source("Lurvy's Shop: Item #5 replacement when bought twice (Wonder Essence)");

        RoomStrings strings(MAP_398, data);

        strings.string(7) = TextboxFormatter(
                R"(\CThat there )" + lurvy_1->item()->name() + R"('ll run you )" + std::to_string(lurvy_1->item()->gold_value()) + R"( Gilder. Okay? \0100\Y)"
        ).format();

        strings.string(8) = TextboxFormatter(
                R"(\C)" + lurvy_2->item()->name() + R"('ll run ya )" + std::to_string(lurvy_2->item()->gold_value()) + R"( Gilder these days. Soun' like a deal?\0100\Y)"
        ).format();

        strings.string(34) = TextboxFormatter(
                R"(\C)" + lurvy_3->item()->name() + R"( costs )" + std::to_string(lurvy_3->item()->gold_value()) + R"( Gilder 'round here.\0100\Y)"
        ).format();

        strings.string(9) = TextboxFormatter(
                R"(\C)" + lurvy_4->item()->name() + R"( costs )" + std::to_string(lurvy_4->item()->gold_value()) + R"( Gilder. Ya still in the market for one?\0100\Y)"
        ).format();

        strings.string(17) = TextboxFormatter(
                R"(\CThat there's a )" + lurvy_5_1->item()->name() + R"(. It's a steal of a deal, at only )" + std::to_string(lurvy_5_1->item()->gold_value()) + R"( Gilder. Deal?\0100\Y)"
        ).format();

        strings.string(18) = TextboxFormatter(
                R"(\CI don't need that )" + lurvy_5_2->item()->name() + R"(. )" + std::to_string(lurvy_5_2->item()->gold_value()) + R"( Gilder'n the thing's yurs. Want it?\0100\Y)"
        ).format();

        strings.string(19) = TextboxFormatter(
                R"(\C)" + lurvy_5_3->item()->name() + R"(, hm? How 'bout )" + std::to_string(lurvy_5_3->item()->gold_value()) + R"( Gilder?\0100\Y)"
        ).format();

        strings.string(35) = R"(\BReceived )" + lurvy_5_1->item()->name() + ".";
        strings.string(36) = R"(\BReceived )" + lurvy_5_2->item()->name() + ".";

        strings.apply_on_data(data);
    }

    static void apply_nadia_shop_text(BinaryFile& data, const RandomizerWorld& world)
    {
        ItemSource* nadia_1 = world.item_source("Nadia's Shop: Item #1 (Herbs)");
        ItemSource* nadia_2 = world.item_source("Nadia's Shop: Item #2 (Strength Tonic)");
        ItemSource* nadia_3 = world.item_source("Nadia's Shop: Item #3 (Magic Elixyr)");
        ItemSource* nadia_4 = world.item_source("Nadia's Shop: Item #4 (Life Vessel)");

        RoomStrings strings(MAP_258, data);

        strings.string(1) = TextboxFormatter(
                R"(\DIf you'd like to purchase )" + nadia_1->item()->name() + R"(, the cost will be )" +
                std::to_string(nadia_1->item()->gold_value()) + R"( Gilder. Is that satisfactory? \0100\Y)"
        ).format();

        strings.string(2) = TextboxFormatter(
                R"(\DAh, yes! )" + nadia_2->item()->name() + R"(! The prime choice of adventurers such as yourself! It costs, )" +
                std::to_string(nadia_2->item()->gold_value()) + R"( Gilder, ok? \0100\Y)"
        ).format();

        strings.string(3) = TextboxFormatter(
                R"(\D)" + nadia_3->item()->name() + "! For just " + std::to_string(nadia_3->item()->gold_value())+
                R"( Gilder, it can be put to work to make all your dreams come true! Want it? \0100\Y)"
        ).format();

        strings.string(4) = TextboxFormatter(
                R"(\D)" + nadia_4->item()->name() + "! "
                "Hmm, pricey, but a prime choice for the especially discriminating. "
                "It's " + std::to_string(nadia_4->item()->gold_value()) + R"( Gilder, ok? \0100\Y)"
        ).format();

        strings.string(57) = R"(\BReceived a )" + nadia_4->item()->name() + R"(.)";

        strings.apply_on_data(data);
    }
};

