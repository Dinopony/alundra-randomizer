#pragma once

#include <vector>
#include <string>
#include <random>
#include "model/world.hpp"

#include "randomizer_options.hpp"
#include "world_solver.hpp"
#include "model/randomizer_world.hpp"

class Item;
class ItemSource;
class HintSource;
class SpawnLocation;

class WorldShuffler
{
private:
    RandomizerWorld& _world;
    WorldSolver _solver;
    const RandomizerOptions& _options;
    std::mt19937 _rng;

    std::vector<Item*> _item_pool;
    std::map<uint8_t, uint8_t> _item_pool_quantities;

    std::vector<Item*> _minimal_items_to_complete;
    std::vector<ItemSource*> _logical_playthrough;

    std::vector<WorldRegion*> _hintable_region_requirements;
    std::vector<uint8_t> _hintable_item_requirements;
    std::vector<uint8_t> _hintable_item_locations;

public:
    WorldShuffler(RandomizerWorld& world, const RandomizerOptions& options);
    
    void randomize();

    [[nodiscard]] Json playthrough_as_json() const;
    [[nodiscard]] Json& debug_log_as_json() { return _solver.debug_log(); }

private:
    // Item randomization
    void randomize_items();
    void init_item_pool();
    ItemSource* place_progression_item_randomly(Item* item, std::vector<ItemSource*> possible_sources);
    void fill_item_source_randomly(ItemSource* source);
    [[nodiscard]] bool test_item_source_compatibility(ItemSource* source, Item* item) const;
    [[nodiscard]] std::vector<WorldPath*> build_weighted_blocked_paths_list();
    void open_random_blocked_path();
    void place_remaining_items();
};
