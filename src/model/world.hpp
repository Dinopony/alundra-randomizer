#pragma once

#include "../tools/json.hpp"
#include "../constants/item_codes.hpp"
#include "../tools/flag.hpp"

#include <map>
#include <vector>

class Item;

class World
{
private:
    std::vector<Item*> _items;
    std::vector<Flag> _starting_flags;

public:
    World();
    ~World();

    [[nodiscard]] const std::vector<Item*>& items() const { return _items; }
    [[nodiscard]] Item* item(uint8_t id) const { return _items.at(id); }
    [[nodiscard]] Item* item(const std::string& name) const;
    void add_item(Item* item);

    [[nodiscard]] const std::vector<Flag>& starting_flags() const { return _starting_flags; }
    void add_starting_flag(const Flag& flag) { _starting_flags.emplace_back(flag); }

private:
    void init_starting_flags();
};
