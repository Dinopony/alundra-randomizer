#pragma once

#include "game_patch.hpp"
#include "../tools/byte_array.hpp"
#include "../game/room_strings.hpp"
#include "../model/hint_source.hpp"
#include "../constants/map_codes.hpp"
#include "../tools/stringtools.hpp"
#include "../tools/textbox_formatter.hpp"

class PatchApplyHintText : public GamePatch
{
public:
    void alter_datas_file(BinaryFile& data, const GameData& game_data, const RandomizerWorld& world) override
    {
        apply_lars_crypt_hint(data, world);
        apply_reptiles_lair_hint(data, world);
        apply_nirudes_lair_hint(data, world);
        apply_coastal_cave_hint(data, world);
        apply_torla_mountain_hint(data, world);
        apply_magyscar_hint(data, world);
        apply_coal_mine_hint(data, world);
        apply_ancient_shrine_hint(data, world);
        apply_fairy_pond_hint(data, world);
        apply_great_tree_hint(data, world);
        //apply_navas_keep_hint(data, world);

        apply_jess_hint(data, world);
        // apply_book_of_elna_hint(data, world);
    }

private:
    static void apply_lars_crypt_hint(BinaryFile& data, const RandomizerWorld& world)
    {
        const std::string& hint_text = world.hint_source("lars_crypt")->text();

        RoomStrings strings(MAP_LARS_CRYPT_25, data);

        strings.string(35) = TextboxFormatter(
                R"(\CYou need to know... )" + hint_text + R"(.)"
        ).format();
        strings.string(37) = R"(\CGood luck on your quest.)";

        strings.apply_on_data(data);
    }

    static void apply_reptiles_lair_hint(BinaryFile& data, const RandomizerWorld& world)
    {
        const std::string& hint_text = world.hint_source("reptiles_lair")->text();

        RoomStrings strings(MAP_REPTILES_LAIR_85, data);

        strings.string(59) = TextboxFormatter(
                R"(\CHear my words, Releaser. I have come to be known as Jeal. I am also one of the seven guardians\A)"
                R"(I was a metalsmith who sought to understand the mysteries of life.\N\A)"
                R"(To stop the evil present in the mortal realm, I sacrificed my human form and vanished into time.\A)"
                R"(I have given thought to trusting the light of your soul to save this weary land.\A)"
                R"(Here is a hint to help you in your journey: )" + hint_text + R"(.)"
        ).format();

        strings.apply_on_data(data);
    }

    static void apply_nirudes_lair_hint(BinaryFile& data, const RandomizerWorld& world)
    {
        const std::string& hint_text = world.hint_source("nirudes_lair")->text();

        RoomStrings strings(MAP_OVERWORLD_E1, data);

        strings.string(37) = TextboxFormatter(
                R"(\CLord Nirude left a message for you with his dying breath: )" + hint_text + R"(.)"
        ).format();

        strings.apply_on_data(data);
    }

    static void apply_coastal_cave_hint(BinaryFile& data, const RandomizerWorld& world)
    {
        const std::string& hint_text = world.hint_source("coastal_cave")->text();

        RoomStrings strings(MAP_COASTAL_CAVE_76, data);
        strings.string(11) = TextboxFormatter(
                R"(\CYou are the Releaser of which\NLars has spoken.\N\A)"
                R"(I sense the power that flows within you. I am Vul, one of the seven guardians.\A)"
                R"(I have seen your power, and sense that you may possess the strength to one day crush Melzas.\A)"
                R"(Listen to my words as they will help you to find what you seek: \A)"
                + hint_text + R"(.\N)"
                R"(Use it well, Releaser\W2you must recover the crests and beat Melzas!\N)"
        ).format();
        strings.apply_on_data(data);
    }

    static void apply_torla_mountain_hint(BinaryFile& data, const RandomizerWorld& world)
    {
        const std::string& hint_text = world.hint_source("torla_mountain")->text();

        RoomStrings strings(MAP_TORLA_MOUNTAIN_448, data);

        strings.string(10) = TextboxFormatter(
                R"(\FUngh\T\W2\T\W2 You humans have grown so strong, too strong, during my long rest.\A)"
                R"(I learnt this during my sleep: )" + hint_text + R"(.\A)"
                R"(I need to go now, Releaser.\AThis time,\W2for eternity...)"
        ).format();

        strings.apply_on_data(data);
    }

    static void apply_magyscar_hint(BinaryFile& data, const RandomizerWorld& world)
    {
        const std::string& hint_text = world.hint_source("magyscar")->text();

        RoomStrings strings(MAP_MAGYSCAR_95, data);

        strings.string(7) = TextboxFormatter(
                R"(\CI am Uma, Queen of Life and Death, and one of the seven Guardians.\A)"
                R"(The Murgg came before you and stole the Crest. My servant was so angry he mistakenly attacked you.\A)"
                R"(Please accept my apologies for his rash actions. The Murgg were talking about something...\A)"
                R"(What they said might help you: ")" + hint_text + R"("\A)"
                R"(Go forth now, Releaser. Return the stolen Crests and destroy Melzas.)"
        ).format();

        strings.apply_on_data(data);
    }

    static void apply_coal_mine_hint(BinaryFile& data, const RandomizerWorld& world)
    {
        const std::string& hint_text = world.hint_source("coal_mine")->text();

        RoomStrings strings(MAP_COAL_MINE_329, data);

        strings.string(46) = TextboxFormatter(
                R"(\FWhat? You haven't found a Crest here?\A)"
                R"(WUKIKI! You remember what Lord Melzas said? ")" + hint_text + R"("\A)"
                R"(GO and do not return without the remaining Crests! WUKIKI!")"
        ).format();

        strings.apply_on_data(data);
    }

    static void apply_ancient_shrine_hint(BinaryFile& data, const RandomizerWorld& world)
    {
        const std::string& hint_text = world.hint_source("ancient_shrine")->text();

        RoomStrings strings(MAP_ANCIENT_SHRINE_34, data);

        strings.string(14) = TextboxFormatter(
                R"(\CYou are truly worthy of the power you possess. Your courage shall not go unrewarded.\A)"
                R"(You deserve to know these ancestral words: ")" + hint_text + R"(")"
        ).format();

        strings.apply_on_data(data);
    }

    static void apply_fairy_pond_hint(BinaryFile& data, const RandomizerWorld& world)
    {
        const std::string& hint_text = world.hint_source("fairy_pond")->text();

        RoomStrings strings(MAP_FAIRY_POND_161, data);
        strings.string(95) = TextboxFormatter(
                R"(\DI am at once amazed and proud that you have progressed this far, Releaser.\A)"
                R"(I am Stenia, Queen of the waters. Your actions have been shown to my heart by the rivers and oceans.\A)"
                R"(I have been waiting for you to come, for I am able to help you in your quest.\A)"
                R"(Mark the words I am about to tell you: )" + hint_text + R"(.)"
        ).format();

        strings.apply_on_data(data);
    }

    static void apply_great_tree_hint(BinaryFile& data, const RandomizerWorld& world)
    {
        const std::string& hint_text = world.hint_source("great_tree")->text();

        RoomStrings strings(MAP_GREAT_TREE_156, data);

        strings.string(48) = TextboxFormatter(
                R"(\EAhh! Furless one, you have come so far, but this is the end of your sad journey of self delusion.\A)"
                R"(Since you are going to die, I can tell you the missing piece in the puzzle...\A)"
                + hint_text + R"(!\A)"
                R"(But it doesn't matter because now is the time where you DIE! WUKIKI!)"
        ).format();

        strings.apply_on_data(data);
    }

    /*
    static void apply_navas_keep_hint(BinaryFile& data, const RandomizerWorld& world)
    {
        const std::string& hint_text = world.hint_source("navas_keep")->text();

        //RoomStrings strings(MAP_NAVAS_CABIN, data);
        // TODO
        //strings.apply_on_data(data);
    }
    */

    static void apply_jess_hint(BinaryFile& data, const RandomizerWorld& world)
    {
        const std::string& hint_text = world.hint_source("jess")->text();

        RoomStrings strings(MAP_JESS_HOUSE, data);

        strings.string(0) = TextboxFormatter(
                R"(\CHello there, Alundra! This morning, I heard something in town.\A)"
                R"(Apparently, )" + hint_text + R"(. Can you believe it?)"
        ).format();

        strings.apply_on_data(data);
    }

    /*
    static void apply_book_of_elna_hint(BinaryFile& data, const RandomizerWorld& world)
    {
        const std::string& hint_text = world.hint_source("book_of_elna");

        // TODO
    }*/
};

