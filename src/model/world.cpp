#include "world.hpp"

#include "item.hpp"

World::World()
{
    _items.resize(ITEM_COUNT, nullptr);
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
