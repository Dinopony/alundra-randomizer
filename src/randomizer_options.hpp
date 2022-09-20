#pragma once

#include <map>
#include <string>
#include <stdexcept>
#include <chrono>
#include <fstream>

#include "tools/json.hpp"
#include "constants/item_codes.hpp"
#include "tools/stringtools.hpp"
#include "tools/argument_dictionary.hpp"

class Item;

class RandomizerOptions 
{
private:
    std::array<std::string, ITEM_COUNT> _item_names;

    // ------------- Game patching settings -------------
    // (included in permalink, presets & plandos)
    bool _original_game_balance = true;
    bool _megaliths_enabled_on_start = true;
    bool _skip_last_dungeon = true;

    // ------------- Randomization settings -------------
    // (included in permalink & presets, not in plandos)
    uint32_t _seed = 0;
    bool _allow_spoiler_log = true;
    std::array<uint8_t, ITEM_COUNT> _items_distribution {};

public:
    explicit RandomizerOptions(const ArgumentDictionary& args, std::array<std::string, ITEM_COUNT>  item_names);

    [[nodiscard]] Json to_json() const;
    void apply_json(const Json& json);

    // Randomization options
    [[nodiscard]] uint32_t seed() const { return _seed; }
    [[nodiscard]] bool allow_spoiler_log() const { return _allow_spoiler_log; }
    [[nodiscard]] const std::array<uint8_t, ITEM_COUNT>& items_distribution() const { return _items_distribution; }

    [[nodiscard]] bool original_game_balance() const { return _original_game_balance; }
    [[nodiscard]] bool megaliths_enabled_on_start() const { return _megaliths_enabled_on_start; }
    [[nodiscard]] bool skip_last_dungeon() const { return _skip_last_dungeon; }

    [[nodiscard]] std::vector<std::string> hash_words() const;
    [[nodiscard]] std::string hash_sentence() const { return stringtools::join(this->hash_words(), " "); }
    [[nodiscard]] std::string permalink() const;

private:
    void apply_game_settings_json(const Json& json);
    void apply_randomizer_settings_json(const Json& json);

    void parse_permalink(std::string permalink);
    void validate() const;
};
