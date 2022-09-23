#include "world_shuffler.hpp"

#include "game/game_data.hpp"
#include "constants/item_codes.hpp"

#include "tools/stringtools.hpp"
#include "tools/vectools.hpp"
#include "tools/exception.hpp"

#include "model/item_source.hpp"
#include "model/world_node.hpp"
#include "model/world_path.hpp"
#include "model/world_region.hpp"

#include "world_solver.hpp"

#include <algorithm>
#include <iostream>

WorldShuffler::WorldShuffler(RandomizerWorld& world, const GameData& game_data, const RandomizerOptions& options) :
    _world          (world),
    _game_data      (game_data),
    _solver         (world),
    _options        (options),
    _rng            (_options.seed())
{}

void WorldShuffler::randomize()
{
    this->init_item_pool();

    _solver.setup(_world.spawn_node(), _world.end_node(), { });

    bool explored_new_nodes = true;
    while(explored_new_nodes)
    {
        // Run a solver step to reach a "blocked" state where something needs to be placed in order to continue
        explored_new_nodes = _solver.run_until_blocked();

        // Place one or several key items in order to open a blocked path
        this->open_random_blocked_path();

        // Item sources changed, force the solver to update its inventory
        _solver.update_current_inventory();
    }

    // Place the remaining items from the item pool in the remaining sources
    this->place_remaining_items();

    // Analyse items required to complete the seed
    Json& debug_log = _solver.debug_log();
    _minimal_items_to_complete = _solver.find_minimal_inventory();
    debug_log["requiredItems"] = Json::array();
    for (const Item* item : _minimal_items_to_complete)
        debug_log["requiredItems"].emplace_back(item->name());
}

/**
 * Initialize the _item_pool internal vector which contains all items that need to be placed during process.
 * The item pool must have the exact size of the empty item sources that need to be filled in order to have a
 * consistent 1:1 matching between the two of them.
 * The _item_pool is built from the required ItemDistribution, and already placed items (in case of plandos) are
 * subtracted from the pool.
 */
void WorldShuffler::init_item_pool()
{
    _item_pool.clear();

    size_t filled_item_sources_count = 0;
    for(ItemSource* source : _world.item_sources())
        if(!source->is_empty())
            filled_item_sources_count++;
    if(filled_item_sources_count == _world.item_sources().size())
        return;

    _item_pool.reserve(_world.item_sources().size());

    std::array<uint8_t, ITEM_COUNT> distribution_copy = _options.items_distribution();
    
    // Count quantities already in place
    for(ItemSource* source : _world.item_sources())
    {
        if(source->is_empty())
            continue;

        uint8_t item_id = source->item()->id();
        if(distribution_copy[item_id] == 0)
        {
            throw RandomizerException("There are more " + source->item()->name() +
                                       " already placed than the expected number in the item pool");
        }
        distribution_copy[item_id] -= 1;
    }

    // Build the item pool from the quantities read inside the ItemDistribution objects, and shuffle it
    for(size_t item_id=0 ; item_id < distribution_copy.size() ; ++item_id)
    {
        uint8_t quantity = distribution_copy[item_id];
        for(uint16_t i=0 ; i<quantity ; ++i)
            _item_pool.emplace_back(_game_data.item(item_id));
    }

    // Count the empty item sources, and compare this count to the item pool size to handle invalid cases
    size_t empty_item_sources_count = 0;
    for(ItemSource* source : _world.item_sources())
    {
        if(source->is_empty())
            empty_item_sources_count += 1;
    }

    if(_item_pool.size() > empty_item_sources_count)
    {
        throw RandomizerException("The number of items in item pool is not the same as the number of item sources (" +
                                    std::to_string(_item_pool.size()) + " =/= " +
                                    std::to_string(_world.item_sources().size()) + ")");
    }
    else if(_item_pool.size() < empty_item_sources_count)
    {
        size_t missing_item_count = empty_item_sources_count - _item_pool.size();
        std::cout << "Warning: Item pool (" << _item_pool.size() << " items) is smaller than the item sources pool ("
                                            << _world.item_sources().size() << " item sources).\n"
                                            << "Remaining sources will remain empty.\n\n";

        for(size_t i=0 ; i<missing_item_count ; ++i)
            _item_pool.emplace_back(_game_data.item(ITEM_NONE));
    }

    vectools::shuffle(_item_pool, _rng);
}

/**
 * Find a random ItemSource that is reachable by the solver where the given progression item can be placed.
 * @param item the item to place
 * @param excluded_sources a list of item sources that are instantly rejected
 * @return the item source where the item was placed (null if it could not be placed anywhere)
 */
ItemSource* WorldShuffler::find_item_source_for_progression_item(const Item* item, const std::vector<ItemSource*>& excluded_sources)
{
    constexpr int32_t DEFAULT_ITEM_SOURCE_WEIGHT = 5;
    constexpr int32_t MINIMAL_ITEM_SOURCE_WEIGHT = 1;

    const std::vector<ItemSource*>& reachable_sources = _solver.reachable_item_sources();
   
    std::vector<ItemSource*> weighted_sources;
    weighted_sources.reserve(reachable_sources.size() * DEFAULT_ITEM_SOURCE_WEIGHT);
    for(ItemSource* source : reachable_sources)
    {
        if (vectools::contains(excluded_sources, source))
            continue;
        if(!source->is_empty())
            continue;
        if(!source->can_contain_progression())
            continue;
        if(!test_item_source_compatibility(source, item))
            continue;

        // We perform a weighting of item sources: the more an item source has been considered to place a progression
        // item, the less likely it will become that it is actually selected. This is to counterbalance the fact that
        // item sources in the first sphere tend to be considered many more times than late game ones (since they are
        // reachable from the start) and therefore tend to contain a lot more progression items (which is a bias we want
        // to avoid).
        ++_item_source_sightings[source];

        int32_t weight = std::max(DEFAULT_ITEM_SOURCE_WEIGHT - _item_source_sightings[source], MINIMAL_ITEM_SOURCE_WEIGHT);
        for(size_t i=0 ; i<weight ; ++i)
            weighted_sources.emplace_back(source);
    }

    if(weighted_sources.empty())
    {
        throw RandomizerException("Could not place " + item->name() + " in any of the "
                                + std::to_string(reachable_sources.size()) + " possible sources.");
    }

    vectools::shuffle(weighted_sources, _rng);
    return weighted_sources[0];
}

/**
 * Fill the given item source with a random compatible item from the item pool.
 * If source could not be filled with anything, a warning message will be outputted.
 * @param source the ItemSource to fill
 * @return the Item that was used to fill the ItemSource
 */
void WorldShuffler::fill_item_source_randomly(ItemSource* source)
{
    for(auto it = _item_pool.begin() ; it != _item_pool.end() ; ++it)
    {
        const Item* item = *it;
        if(test_item_source_compatibility(source, item))
        {
            source->item(item);
            _item_pool.erase(it);
            return;
        }
    }

    std::cout << "[WARNING] Item source '" << source->name() << "' could not be filled with any item of the item pool.\n";
}

/**
 * Checks if the given Item can be placed inside the given ItemSource if we follow strictly the ItemDistribution rules.
 * @param source the ItemSource to test
 * @param item the Item to test
 * @return true if the Item can be placed inside the ItemSource, false otherwise
 */
bool WorldShuffler::test_item_source_compatibility(ItemSource* source, const Item* item) const
{
    if(source->forbid_precious_items() && item->is_precious())
        return false;
    return true;
}

/**
 * Checks if the current list of items is contained inside the item pool (and therefore can be placed inside the world).
 * @param items the Item objects to check
 */
bool WorldShuffler::item_pool_contains_items(const std::vector<const Item*>& items) const
{
    std::map<const Item*, uint16_t> item_quantities;
    for(const Item* item : items)
        ++item_quantities[item];

    for(auto& [item, qty] : item_quantities)
        if(vectools::count(_item_pool, item) < qty)
            return false;
    return true;
}

/**
 * Build a list containing the paths currently being blocked as identified by the WorldSolver.
 * This list is weighted (contains as many instances of a WorldPath as its "weight" attribute) and shuffled.
 * @return a list of blocked paths
 */
std::vector<WorldPath*> WorldShuffler::build_weighted_blocked_paths_list()
{
    const std::vector<WorldPath*>& blocked_paths = _solver.blocked_paths();
    std::vector<WorldPath*> weighted_blocked_paths;

    for (WorldPath* path : blocked_paths)
    {
        // If items are not the (only) blocking point for taking this path, let it go
        if(!_solver.missing_nodes_to_take_path(path).empty())
            continue;

        // If all items cannot be placed since the item pool is running out of that item type,
        // do not try to open this path. It will open by itself once the item (as placed inside plando)
        // will be reached.
        std::vector<const Item*> items_to_place = _solver.missing_items_to_take_path(path);
        std::map<uint8_t, uint16_t> quantities_to_place;
        for(const Item* item : items_to_place)
        {
            if(!quantities_to_place.count(item->id()))
                quantities_to_place[item->id()] = 0;
            quantities_to_place[item->id()] += 1;
        }

        bool can_place_all_items = true;
        for(auto& [item_id, quantity_to_place] : quantities_to_place)
        {
            if(vectools::count(_item_pool, _game_data.item(item_id)) < quantity_to_place)
            {
                can_place_all_items = false;
                break;
            }
        }

        if(can_place_all_items)
        {
            for(int i=0 ; i<path->weight() ; ++i)
                weighted_blocked_paths.emplace_back(path);

            // All conditions are met, add this path to the weighted blocked paths list
            Json& debug_log = _solver.debug_log_for_current_step();
            debug_log["blockedPaths"].emplace_back(path->origin()->id() + " --> " + path->destination()->id());
        }
    }

    vectools::shuffle(weighted_blocked_paths, _rng);
    return std::move(weighted_blocked_paths);
}

/**
 * Open one of the blocked paths encountered by the WorldSolver by placing randomly the items required to cross it.
 */
void WorldShuffler::open_random_blocked_path()
{
    Json& debug_log = _solver.debug_log_for_current_step();
    debug_log["blockedPaths"] = Json::array();

    // List all blocked paths, taking weights into account and randomly choose one
    std::vector<WorldPath*> weighted_blocked_paths = this->build_weighted_blocked_paths_list();
    if (weighted_blocked_paths.empty())
        return;

    for(WorldPath* path_to_open : weighted_blocked_paths)
    {
        std::vector<const Item*> items_to_place = _solver.missing_items_to_take_path(path_to_open);

        // Check if all items required to cross the path are available in the item pool
        if(!this->item_pool_contains_items(items_to_place))
            continue;

        // Try to find a set of ItemSources that work to place all the required progression items to open the path
        std::vector<ItemSource*> picked_item_sources;
        for(const Item* item : items_to_place)
        {
            ItemSource* source = this->find_item_source_for_progression_item(item, picked_item_sources);
            if(!source)
                break;

            picked_item_sources.emplace_back(source);
        }

        // We couldn't find a set of sources accepting all the items required for the path, try unlocking another path
        if(picked_item_sources.size() < items_to_place.size())
            continue;

        debug_log["chosenPath"].emplace_back(path_to_open->origin()->id() + " --> " + path_to_open->destination()->id());

        // Place all missing key items for the player to be able to open this blocking path
        for(size_t i=0 ; i<items_to_place.size() ; ++i)
        {
            const Item* item = items_to_place[i];
            ItemSource* source = picked_item_sources[i];
            source->item(item);
            vectools::erase_first(_item_pool, item);

            _logical_playthrough.emplace_back(source);
            debug_log["placedKeyItems"][source->name()] = item->name();
        }

        return;
    }
}

/**
 * Place all remaining Items from the item pool in the ItemSources that are still empty.
 * Since the item pool is build in a way to enforce having the exact same number of Items and ItemSources, the
 * count matching should be perfect inside this function.
 */
void WorldShuffler::place_remaining_items()
{
    Json& debug_log = _solver.debug_log_for_current_step();

    std::vector<ItemSource*> unrestricted_item_sources;
    for(ItemSource* source : _world.item_sources())
    {
        if(!source->is_empty())
            continue;

        unrestricted_item_sources.emplace_back(source);
    }
    vectools::shuffle(unrestricted_item_sources, _rng);

    for(ItemSource* source : unrestricted_item_sources)
        this->fill_item_source_randomly(source);

    for(const Item* item : _item_pool)
        std::cout << "[WARNING] Item '" << item->name() << "' is remaining in the item pool at end of generation.\n";

    _item_pool.clear();
}

Json WorldShuffler::playthrough_as_json() const
{
    Json json;

    // Filter the logical playthrough to keep only strictly needed key items
    for(ItemSource* source : _logical_playthrough)
    {
        const Item* key_item_in_source = source->item();
        if(vectools::contains(_minimal_items_to_complete, key_item_in_source))
            json[source->name()] = key_item_in_source->name();
    }

    return json;
}
