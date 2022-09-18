#include "world.hpp"

#include "item.hpp"
#include "../constants/flags.hpp"

World::World()
{
    _items.resize(ITEM_COUNT, nullptr);

    this->init_starting_flags();
}

World::~World()
{
    for (Item* item : _items)
        delete item;
}

Item* World::item(const std::string& name) const
{
    if(name.empty())
        return nullptr;

    for (Item* item : _items)
        if(item->name() == name)
            return item;

    return nullptr;
}

void World::add_item(Item* item)
{
    _items[item->id()] = item;
}

void World::init_starting_flags()
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
}