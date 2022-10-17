#include "game_data.hpp"

#include "item.hpp"
#include "../constants/flags.hpp"
#include "../randomizer_options.hpp"
#include "../tools/exception.hpp"
#include "data/item.json.hxx"
#include "../tools/vectools.hpp"


GameData::GameData()
{
    this->init_items();
    this->init_starting_flags();
}

const Item* GameData::item(const std::string& name) const
{
    for(const Item& item : _items)
        if(item.name() == name)
            return &item;
    throw RandomizerException("Could not find item with name '" + name + "'");
}

Item* GameData::item(const std::string& name)
{
    for(Item& item : _items)
        if(item.name() == name)
            return &item;
    throw RandomizerException("Could not find item with name '" + name + "'");
}

/**
 * Setup the Item objects representing in-game items
 */
void GameData::init_items()
{   
    for(size_t i=0 ; i<_items.size() ; ++i)
        _items[i].id(i);

    Json items_json = Json::parse(ITEMS_JSON);
    uint8_t item_id = 0;
    for(Json& item_json : items_json) // auto& [item_name, item_json] : items_json.items()
    {
        _items[item_id].apply_json(item_json);
        ++item_id;
    }
}

/**
 * Setup the starting flags set on New Game, depending on the randomizer options.
 * 
 * @param options the randomizer options
 */
void GameData::init_starting_flags()
{
    // Mark HUD as allowed (0x40 -> 0x1DD403)
    _starting_flags.emplace_back(FLAG_HUD_ALLOWED);

    // Remove Septimus cutscene telling us Ronan has gone insane (0x01 -> 0x1DD358)
    _starting_flags.emplace_back(FLAG_SEPTIMUS_WAKE_UP_TALKING_ABOUT_RONAN);

    // Remove Sybill cutscene showing Holy Sword cutscene (0x60 -> 0x1DD3F3)
    _starting_flags.emplace_back(FLAG_SYBILL_HOLY_SWORD_VISION_1);
    _starting_flags.emplace_back(FLAG_SYBILL_HOLY_SWORD_VISION_2);

    // Open Underground Waterway (0x10 -> 0x1DD370)
    _starting_flags.emplace_back(FLAG_UNDERGROUND_WATERWAY_OPENED);

    // Remove the fallen tree on the way to Cliffs of Madness (0x20 -> 0x1DD35F)
    _starting_flags.emplace_back(FLAG_REMOVED_TREE_TO_CLIFFS_OF_MADNESS);

    // Change the sign on the way to Cliffs of Madness to remove the fallen tree warning (0x08 -> 0x1DD34F)
    _starting_flags.emplace_back(FLAG_CHANGE_SIGN_TO_CLIFFS_OF_MADNESS);

    // Make Merrick trade Gilded Falcons right away
    _starting_flags.emplace_back(FLAG_LEARNED_ABOUT_MERRICK_WANTING_FALCONS);
    _starting_flags.emplace_back(FLAG_TALKED_WITH_MERRICK);
    _starting_flags.emplace_back(FLAG_TALKED_WITH_MERRICK_ABOUT_FALCONS);
    _starting_flags.emplace_back(FLAG_ACCEPTED_MERRICK_DEAL_ABOUT_FALCONS);

    // Allow entering in Magyscar using a Bomb
    _starting_flags.emplace_back(FLAG_MAGYSCAR_WALL_IS_CRACKED);

    // Activate Torla geysers on game start, giving access to Torla Mountain
    _starting_flags.emplace_back(FLAG_TORLA_GEYSERS_ACTIVE);

    // Activate Nirude moai statues
    // TODO: This looks like it's breaking the boss reward for now, so we'll just let the normal dungeon flow
    //       do the work.
    // _starting_flags.emplace_back(FLAG_SAW_NIRUDE_STATUES_ACTIVATION_CUTSCENE);
    // _starting_flags.emplace_back(FLAG_NIRUDE_SAW_TOP_LEFT_CUTSCENE);
    // _starting_flags.emplace_back(FLAG_NIRUDE_STATUES_VULNERABLE);
    _starting_flags.emplace_back(FLAG_NIRUDE_LOWER_BOULDER_REMOVED);

    // Flag allowing to exit Lars Crypt without having to beat the boss
    _starting_flags.emplace_back(FLAG_CAN_EXIT_LARS_CRYPT);
    // Put flowers on other tombs to indicate you need the Bouquet to enter Lars Crypt
    _starting_flags.emplace_back(FLAG_OTHER_FLOWERS_ARE_IN_GRAVEYARD);

    // Remove the specific cutscene when we enter Lurvy's shop for the first time
    _starting_flags.emplace_back(FLAG_LURVY_WELCOMED_US_ONCE);

    // Open Yustel's house in Inoa, and place her behind her counter as if we already talked with her once
    _starting_flags.emplace_back(FLAG_TALKED_TO_MEIA_PUTTING_SEPTIMUS_AT_SANCTUARY);
    _starting_flags.emplace_back(FLAG_TALKED_TO_YUSTEL_ONCE);
    _starting_flags.emplace_back(FLAG_YUSTEL_MOVED_BEHIND_COUNTER);
}

void GameData::apply_options(const RandomizerOptions& options)
{
    if(options.megaliths_enabled_on_start())
        _starting_flags.emplace_back(FLAG_CAN_ACTIVATE_MEGALITHS);

    if(options.skip_last_dungeon())
    {
        _starting_flags.emplace_back(FLAG_OPENED_LAKE_SHRINE_EXTERIOR_MAIN_GATE);
        _starting_flags.emplace_back(FLAG_LAKE_SHRINE_EXTERIOR_RIGHT_GARDEN_BUTTON);
        _starting_flags.emplace_back(FLAG_LAKE_SHRINE_EXTERIOR_LEFT_GARDEN_BUTTON);
        _starting_flags.emplace_back(FLAG_LAKE_SHRINE_EXTERIOR_GARDEN_MIDDLE_GATE);
        _starting_flags.emplace_back(FLAG_LAKE_SHRINE_INTERIOR_MELZAS_INTRO_CUTSCENE_1);
        _starting_flags.emplace_back(FLAG_LAKE_SHRINE_INTERIOR_MELZAS_INTRO_CUTSCENE_2);
        _starting_flags.emplace_back(FLAG_LAKE_SHRINE_NW_SEAL_BROKEN);
        _starting_flags.emplace_back(FLAG_LAKE_SHRINE_W_SEAL_BROKEN);
        _starting_flags.emplace_back(FLAG_LAKE_SHRINE_SW_SEAL_BROKEN);
        _starting_flags.emplace_back(FLAG_LAKE_SHRINE_SE_SEAL_BROKEN);
        _starting_flags.emplace_back(FLAG_LAKE_SHRINE_E_SEAL_BROKEN);
        _starting_flags.emplace_back(FLAG_LAKE_SHRINE_NE_SEAL_BROKEN);
    }

    this->handle_crests(options);
}

void GameData::handle_crests(const RandomizerOptions& options)
{
    std::vector<uint8_t> crests = {
            ITEM_RUBY_CREST, ITEM_SAPPHIRE_CREST, ITEM_TOPAZ_CREST, ITEM_AGATE_CREST,
            ITEM_EMERALD_CREST, ITEM_GARNET_CREST, ITEM_DIAMOND_CREST
    };
    std::mt19937 rng(options.seed());
    vectools::shuffle(crests, rng);

    // Add random crests to the "used crests" list
    for(size_t i=0 ; i<options.crest_count() ; ++i)
        _used_crests.emplace_back(crests[i]);

    // For each excluded crest, add the flag to automatically place it on its pedestal on game start
    const std::map<uint8_t, Flag> crest_flags = {
            { ITEM_RUBY_CREST,      FLAG_PLACED_RUBY_CREST },
            { ITEM_SAPPHIRE_CREST,  FLAG_PLACED_SAPPHIRE_CREST },
            { ITEM_TOPAZ_CREST,     FLAG_PLACED_TOPAZ_CREST },
            { ITEM_AGATE_CREST,     FLAG_PLACED_AGATE_CREST },
            { ITEM_EMERALD_CREST,   FLAG_PLACED_EMERALD_CREST },
            { ITEM_GARNET_CREST,    FLAG_PLACED_GARNET_CREST },
            { ITEM_DIAMOND_CREST,   FLAG_PLACED_DIAMOND_CREST }
    };

    for(size_t i=options.crest_count() ; i<crests.size() ; ++i)
    {
        uint8_t crest_id = crests[i];
        _starting_flags.emplace_back(crest_flags.at(crest_id));
    }
}
