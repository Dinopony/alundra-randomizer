#pragma once

#include "../constants/item_codes.hpp"
#include "../constants/item_names.hpp"
#include "../tools/json.hpp"
#include "../tools/flag.hpp"
#include "item.hpp"

#include <map>
#include <vector>

class RandomizerOptions;

class GameData
{
private:
    std::array<Item, ITEM_COUNT> _items;
    std::vector<Flag> _starting_flags;

public:
    GameData(const RandomizerOptions& options);

    [[nodiscard]] const std::array<Item, ITEM_COUNT>& items() const { return _items; }
    [[nodiscard]] const Item* item(uint8_t id) const { return &(_items[id]); }
    [[nodiscard]] Item* item(uint8_t id) { return &(_items[id]); }
    [[nodiscard]] const Item* item(const std::string& name) const { return this->item(get_item_id_from_name(name)); }
    [[nodiscard]] Item* item(const std::string& name) { return this->item(get_item_id_from_name(name)); }

    [[nodiscard]] const std::vector<Flag>& starting_flags() const { return _starting_flags; }
    void add_starting_flag(const Flag& flag) { _starting_flags.emplace_back(flag); }

private:
    void init_items();
    void init_starting_flags(const RandomizerOptions& options);
};
