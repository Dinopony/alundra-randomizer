#pragma once

#include <vector>
#include <string>
#include <random>
#include <array>

#include "randomizer_options.hpp"
#include "world_solver.hpp"
#include "model/randomizer_world.hpp"
#include "constants/item_codes.hpp"

class Item;
class ItemSource;
class HintSource;
class SpawnLocation;

class WorldShuffler
{
private:
    RandomizerWorld& _world;
    const GameData& _game_data;
    const RandomizerOptions& _options;

    /// The random number generator (RNG) used to randomize the world
    std::mt19937 _rng;

    /// The world solver used to explore progressively the world and know where we can put things, and
    /// what we can put in order to actually unlock path and make progress
    WorldSolver _solver;

    /// The pool of items to place (contains duplicates for the same item type to reperesent the amount
    /// of item instances to place)
    std::vector<const Item*> _item_pool;

    /// The list of item sources containing progression items, in the order they were filled by the shuffler
    /// algorithm. This means it contains (more or less) the minimal list of item sources to take in order to
    /// be able to complete the seed
    std::vector<ItemSource*> _logical_playthrough;

    /// The smallest inventory the solver could find to beat the seed. This is used to filter the item sources
    /// from `_logical_playthrough` to only keep those containing relevant items.
    std::vector<const Item*> _minimal_items_to_complete;

public:
    WorldShuffler(RandomizerWorld& world, const GameData& game_data, const RandomizerOptions& options);
    
    void randomize();

    [[nodiscard]] Json playthrough_as_json() const;
    [[nodiscard]] Json& debug_log_as_json() { return _solver.debug_log(); }

private:
    void init_item_pool();

    ItemSource* place_progression_item_randomly(const Item* item, std::vector<ItemSource*> possible_sources);
    void fill_item_source_randomly(ItemSource* source);
    [[nodiscard]] bool test_item_source_compatibility(ItemSource* source, const Item* item) const;
    [[nodiscard]] std::vector<WorldPath*> build_weighted_blocked_paths_list();
    void open_random_blocked_path();
    void place_remaining_items();
};
