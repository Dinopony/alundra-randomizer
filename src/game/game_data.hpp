#pragma once

#include "../constants/item_codes.hpp"
#include "../tools/json.hpp"
#include "../tools/flag.hpp"
#include "item.hpp"

#include <map>
#include <vector>

class RandomizerOptions;

class GameData
{
private:
    /// An array with structures representing the various items that can be found in the game 
    std::array<Item, ITEM_COUNT> _items;

    /// A list of starting flags that are activated on New Game
    std::vector<Flag> _starting_flags;

    /// A list containing the item IDs of the crests that will have to be found to complete the game
    std::vector<uint8_t> _used_crests;

public:
    GameData();

private:
    void init_items();
    void init_starting_flags();

public:
    void apply_options(const RandomizerOptions& options);

    [[nodiscard]] const std::array<Item, ITEM_COUNT>& items() const { return _items; }
    [[nodiscard]] const Item* item(uint8_t id) const { return &(_items[id]); }
    [[nodiscard]] Item* item(uint8_t id) { return &(_items[id]); }
    [[nodiscard]] const Item* item(const std::string& name) const;
    [[nodiscard]] Item* item(const std::string& name);

    [[nodiscard]] const std::vector<Flag>& starting_flags() const { return _starting_flags; }
    void add_starting_flag(const Flag& flag) { _starting_flags.emplace_back(flag); }

    [[nodiscard]] const std::vector<uint8_t>& used_crests() const { return _used_crests; }

private:
    void handle_crests(const RandomizerOptions& options);
};
