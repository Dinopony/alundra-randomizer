#pragma once

#include "../tools/json.hpp"
#include "../constants/item_codes.hpp"

#include <map>
#include <vector>

class Item;

class World
{
private:
    std::vector<Item*> _items;

public:
    World();
    ~World();

    [[nodiscard]] const std::vector<Item*>& items() const { return _items; }
    [[nodiscard]] Item* item(uint8_t id) const { return _items.at(id); }
    [[nodiscard]] Item* item(const std::string& name) const;
    void add_item(Item* item);
};
