#include "game_data.hpp"

#include "item.hpp"
#include "../constants/flags.hpp"
#include "data/item.json.hxx"
#include "../randomizer_options.hpp"

GameData::GameData(const RandomizerOptions& options)
{
    this->init_items();
    this->init_starting_flags(options);
}

void GameData::init_starting_flags(const RandomizerOptions& options)
{
    // Mark HUD as allowed (0x40 -> 0x1DD403)
    _starting_flags.emplace_back(FLAG_HUD_ALLOWED);

    // Remove Septimus cutscene telling us Ronan has gone insane (0x01 -> 0x1DD358)
    _starting_flags.emplace_back(FLAG_SEPTIMUS_WAKE_UP_TALKING_ABOUT_RONAN);

    // Remove Sybill cutscene showing Holy Sword cutscene (0x60 -> 0x1DD3F3)
    _starting_flags.emplace_back(FLAG_SYBILL_HOLY_SWORD_VISION_1);
    _starting_flags.emplace_back(FLAG_SYBILL_HOLY_SWORD_VISION_2);

    // Open Underground Waterway (0x10 -> 0x1DD370)
    _starting_flags.emplace_back(FLAG_UNDEGROUND_WATERWAY_OPENED);

    // Remove the fallen tree on the way to Cliffs of Madness (0x20 -> 0x1DD35F)
    _starting_flags.emplace_back(FLAG_REMOVED_TREE_TO_CLIFFS_OF_MADNESS);

    // Change the sign on the way to Cliffs of Madness to remove the fallen tree warning (0x08 -> 0x1DD34F)
    _starting_flags.emplace_back(FLAG_CHANGE_SIGN_TO_CLIFFS_OF_MADNESS);

    // Make Merrick trade Gilded Falcons right away
    _starting_flags.emplace_back(FLAG_LEARNED_ABOUT_MERRICK_WANTING_FALCONS);
    _starting_flags.emplace_back(FLAG_TALKED_WITH_MERRICK);
    _starting_flags.emplace_back(FLAG_TALKED_WITH_MERRICK_ABOUT_FALCONS);
    _starting_flags.emplace_back(FLAG_ACCEPTED_MERRICK_DEAL_ABOUT_FALCONS);
    
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
}

void GameData::init_items()
{   
    for(size_t i=0 ; i<_items.size() ; ++i)
    {
        Item& item = _items[i];
        item.id(i);
        item.name(get_item_name_from_id(i));
    }

    Json items_json = Json::parse(ITEMS_JSON);
    for(auto& [item_name, item_json] : items_json.items())
    {
        uint8_t item_id = get_item_id_from_name(item_name);
        _items[item_id].apply_json(item_json);
    }
}
