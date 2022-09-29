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
class GameData;
class RandomizerWorld;

class RandomizerOptions 
{
private:
    // ------------- Game patching settings -------------------------------------

    /// If true, the game balance from the japanese version is used instead of the balance edited
    /// by Working Designs for the US version. This mostly changes enemies HP and damage.
    bool _original_game_balance = true;

    /// If true, the player won't have to do Berue's memory minigame to activate megaliths, as they
    /// will be activated on game start. Player still has to visit each megalith to unlock the ability
    /// to come back there later.
    bool _megaliths_enabled_on_start = true;

    /// If true, all gates in Lake Shrine exterior & interior will be open right away, allowing the player
    /// to simply go forward to reach the final boss once all crests are collected & placed
    bool _skip_last_dungeon = true;

    /// If true, each boots give its own effects instead of boots tiers giving all previous tiers effects
    bool _split_boots_effects = true;

    /// If true, equipment with tiers (weapons, armors and magic)
    bool _progressive_items = true;

    /// The amount of times you need to die in order to be able to get King Snow's item
    uint8_t _king_snow_death_count = 20;

    // ------------- Randomization settings -------------

    /// The random number generator's seed, determining all of the randomization. Generating two seeds
    /// with the same settings and the same seed should produce the exact same result.
    uint32_t _seed = 0;

    /// If false, the randomizer program will forbid the generation of a full spoiler log, and will only
    /// output the initial settings in that file instead of the full contents of all ItemSources.
    bool _allow_spoiler_log = true;

    /// An array containing the items distribution (the complete set of items that will be placed inside
    /// ItemSources during randomization). The total count must always be lower or equal to the number
    /// of ItemSources inside the world.
    std::array<uint8_t, ITEM_COUNT> _items_distribution {};

    // ------------- World descriptor -------------

    /// A map used to define item sources that will **always** contain the same item (keys are ItemSource IDs,
    /// values are Item IDs)
    std::map<uint16_t, uint8_t> _fixed_item_sources;

    /// A map used to define item sources that will **always** contain the same item (keys are ItemSource IDs,
    /// values are Item IDs)
    std::vector<uint16_t> _item_sources_without_progression;

public:
    explicit RandomizerOptions(const ArgumentDictionary& args, const GameData& game_data, const RandomizerWorld& world);

    [[nodiscard]] Json to_json(const GameData& game_data, const RandomizerWorld& world) const;
    void apply_json(const Json& json, const GameData& game_data, const RandomizerWorld& world);

    // Randomization options
    [[nodiscard]] uint32_t seed() const { return _seed; }
    [[nodiscard]] bool allow_spoiler_log() const { return _allow_spoiler_log; }
    [[nodiscard]] const std::array<uint8_t, ITEM_COUNT>& items_distribution() const { return _items_distribution; }

    [[nodiscard]] bool original_game_balance() const { return _original_game_balance; }
    [[nodiscard]] bool megaliths_enabled_on_start() const { return _megaliths_enabled_on_start; }
    [[nodiscard]] bool skip_last_dungeon() const { return _skip_last_dungeon; }
    [[nodiscard]] bool split_boots_effects() const { return _split_boots_effects; }
    [[nodiscard]] bool progressive_items() const { return _progressive_items; }
    [[nodiscard]] uint8_t king_snow_death_count() const { return _king_snow_death_count; }

    [[nodiscard]] const std::map<uint16_t, uint8_t>& fixed_item_sources() const { return _fixed_item_sources; }
    [[nodiscard]] const std::vector<uint16_t>& item_sources_without_progression() const { return _item_sources_without_progression; }

    [[nodiscard]] std::vector<std::string> hash_words() const;
    [[nodiscard]] std::string hash_sentence() const { return stringtools::join(this->hash_words(), " "); }
    [[nodiscard]] std::string permalink() const;

private:
    void apply_game_settings_json(const Json& json);
    void apply_randomizer_settings_json(const Json& json, const GameData& game_data, const RandomizerWorld& world);

    void parse_permalink(std::string permalink);
    void validate() const;
};
