#pragma once

#include <vector>

#include "tools/json.hpp"
#include "tools/vectools.hpp"
#include "model/randomizer_world.hpp"

class WorldNode;
class ItemSource;
class Item;
class WorldPath;

/**
 * WorldSolver is a class used for checking a world solvability, or to explore a world
 * until the next blocking state to be able to fill it iteratively and make it solvable
 * in the end.
 * 
 * It works by setting a starting point and an end point, and optional constraints
 * (forbidding to take item X, to take items in node Y...) than can be used in some
 * circumstances to determine minimal requirements to solve the seed.
 */
class WorldSolver
{
private:
    const RandomizerWorld& _world;

    // ------------- Solver input settings -------------

    /// The node where to start from
    WorldNode* _start_node = nullptr;

    /// The node to reach to consider the exploration as complete
    WorldNode* _end_node = nullptr;
    
    /// The starting inventory when beginning to explore from the starting node
    std::vector<const Item*> _starting_inventory;

    /// A list of item quantities for each type we refrain from taking. This is typically
    /// used to determine if a seed is completable without using a specific item.
    std::map<const Item*, uint16_t> _forbidden_items;

    /// A list of nodes where it is forbidden to take any item from any item source.
    /// This is typically used to determine if a region is barren or not.
    std::vector<WorldNode*> _forbidden_nodes_to_pick_items;

    // ------------- Solver state variables -------------

    /// The current inventory during exploration
    std::vector<const Item*> _inventory;

    /// A list of all nodes already explored by the solver
    std::vector<WorldNode*> _explored_nodes;
    
    /// A list of all nodes to explore during the exploration step. This is filled with new nodes
    /// when crossable paths are encountered.
    std::vector<WorldNode*> _nodes_to_explore;
    
    /// A list of all paths we cannot take yet because we are missing an item / a visited node
    std::vector<WorldPath*> _blocked_paths;
    
    /// A list of all currently reachable item sources encountered during our exploration
    std::vector<ItemSource*> _reachable_item_sources;

    /// The current exploration step
    uint32_t _step_count = 0;

    /// A debug log under the form of a dumpable JSON object
    Json _debug_log;

public:
    explicit WorldSolver(const RandomizerWorld& world);

    void forbid_items(const std::map<const Item*, uint16_t>& item_quantities);
    void forbid_item_type(const Item* item);
    void forbid_taking_items_from_nodes(const std::vector<WorldNode*>& forbidden_nodes);

    void setup(WorldNode* start_node, WorldNode* end_node, const std::vector<const Item*>& starting_inventory);
    bool try_to_solve(WorldNode* start_node, WorldNode* end_node, const std::vector<const Item*>& starting_inventory);
    bool run_until_blocked();

    [[nodiscard]] const std::vector<const Item*>& starting_inventory() const { return _starting_inventory; }
    void starting_inventory(const std::vector<const Item*>& starting_inventory) { _starting_inventory = starting_inventory; }
    void update_current_inventory();

    [[nodiscard]] const std::vector<WorldPath*>& blocked_paths() const { return _blocked_paths; }
    [[nodiscard]] const std::vector<ItemSource*>& reachable_item_sources() const { return _reachable_item_sources; }
    [[nodiscard]] const std::vector<const Item*>& inventory() const { return _inventory; }

    [[nodiscard]] bool can_take_path(WorldPath* path) const;
    [[nodiscard]] std::vector<const Item*> missing_items_to_take_path(WorldPath* path) const;
    [[nodiscard]] std::vector<WorldNode*> missing_nodes_to_take_path(WorldPath* path) const;

    [[nodiscard]] bool reached_end() const { return vectools::contains(_explored_nodes, _end_node); }
    [[nodiscard]] std::vector<const Item*> find_minimal_inventory();

    [[nodiscard]] Json& debug_log() { return _debug_log; }
    [[nodiscard]] Json& debug_log_for_current_step() { return _debug_log["steps"][std::to_string(_step_count)]; }

private:
    void expand_exploration_zone();
    bool try_unlocking_paths();
    void take_path(WorldPath* path);
};
