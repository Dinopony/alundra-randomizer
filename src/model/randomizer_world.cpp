#include "randomizer_world.hpp"

#include "../game/game_data.hpp"
#include "world_node.hpp"
#include "world_path.hpp"
#include "world_region.hpp"
#include "hint_source.hpp"
#include "../randomizer_options.hpp"

// Include headers automatically generated from model json files
#include "data/item_source.json.hxx"
#include "data/hint_source.json.hxx"
#include "data/world_node.json.hxx"
#include "data/world_path.json.hxx"
#include "data/world_region.json.hxx"

#include "../tools/exception.hpp"
#include "../tools/vectools.hpp"

#include <iostream>

RandomizerWorld::RandomizerWorld(const GameData& game_data)
{
    this->init_nodes();
    this->init_regions();
    this->init_paths(game_data);
    this->init_item_sources(game_data);
    this->init_hint_sources();
}

RandomizerWorld::~RandomizerWorld()
{
    for (ItemSource* source : _item_sources)
        delete source;
    for (auto& [key, node] : _nodes)
        delete node;
    for (WorldPath* path : _paths)
        delete path;
    for (WorldRegion* region : _regions)
        delete region;
    for (auto& [source_id, source] : _hint_sources)
        delete source;
}

ItemSource* RandomizerWorld::item_source(const std::string& name) const
{
    for (ItemSource* source : _item_sources)
        if (source->pretty_name() == name)
            return source;

    throw std::out_of_range("No ItemSource with name '" + name + "'");
}

uint16_t RandomizerWorld::item_source_id(const ItemSource* source) const
{
    for(size_t i=0 ; i<_item_sources.size() ; ++i)
        if(source == _item_sources[i])
            return (uint16_t)i;

    throw std::out_of_range("ItemSource was not found when looking for its ID");
}

std::vector<ItemSource*> RandomizerWorld::item_sources_with_item(const Item* item) const
{
    std::vector<ItemSource*> sources_with_item;

    for (ItemSource* source : _item_sources)
        if (source->item() == item)
            sources_with_item.emplace_back(source);

    return sources_with_item;
}

std::vector<WorldPath*> RandomizerWorld::paths_starting_from_node(const WorldNode* node) const
{
    std::vector<WorldPath*> ret;
    for(WorldPath* path : _paths)
    {
        if(path->origin() == node || (path->is_two_way() && path->destination() == node))
            ret.emplace_back(path);
    }
    return ret;
}

WorldRegion* RandomizerWorld::region(const std::string& name) const
{
    for(WorldRegion* region : _regions)
        if(region->name() == name)
            return region;
    return nullptr;
}

void RandomizerWorld::init_item_sources(const GameData& game_data)
{
    Json item_sources_json = Json::parse(ITEM_SOURCES_JSON);
    for(const Json& source_json : item_sources_json)
    {
        _item_sources.emplace_back(ItemSource::from_json(source_json, game_data, *this));
    }

#ifdef DEBUG
    std::cout << _item_sources.size() << " item sources loaded." << std::endl;
#endif
}

void RandomizerWorld::init_nodes()
{
    Json nodes_json = Json::parse(WORLD_NODES_JSON);
    for(auto& [node_id, node_json] : nodes_json.items())
    {
        WorldNode* new_node = WorldNode::from_json(node_json, node_id);
        _nodes[node_id] = new_node;
    }

#ifdef DEBUG
    std::cout << _nodes.size() << " nodes loaded." << std::endl;
#endif
}

void RandomizerWorld::init_paths(const GameData& game_data)
{
    Json paths_json = Json::parse(WORLD_PATHS_JSON);
    for(const Json& path_json : paths_json)
    {
        WorldPath* path = WorldPath::from_json(path_json, _nodes, game_data);
        this->add_path(path);
    }

#ifdef DEBUG
    std::cout << _paths.size() << " paths loaded." << std::endl;
#endif
}

void RandomizerWorld::init_regions()
{
    Json regions_json = Json::parse(WORLD_REGIONS_JSON);
    for(const Json& region_json : regions_json)
        _regions.emplace_back(WorldRegion::from_json(region_json, *this));

#ifdef DEBUG
    std::cout << _regions.size() << " regions loaded." << std::endl;
#endif

    // Check for orphan nodes
    for(auto& [id, node] : _nodes)
        if(node->region() == nullptr)
            throw RandomizerException("Node '" + node->id() + "' doesn't belong to any region");
}

void RandomizerWorld::init_hint_sources()
{
    Json hint_sources_json = Json::parse(HINT_SOURCES_JSON);
    for(auto& [hint_source_id, hint_source_json] : hint_sources_json.items())
    {
        _hint_sources[hint_source_id] = HintSource::from_json(hint_source_json, *this);
    }

#ifdef DEBUG
    std::cout << _hint_sources.size() << " hint sources loaded." << std::endl;
#endif
}

void RandomizerWorld::apply_options(const RandomizerOptions& options, const GameData& game_data)
{
    // Apply item sources which contents were set inside the preset configuration
    for(auto& [source_id, item_id] : options.fixed_item_sources())
        _item_sources[source_id]->item(game_data.item(item_id));

    // Mark item sources that cannot contain progression with a specific flag
    for(uint16_t source_id : options.item_sources_without_progression())
        _item_sources[source_id]->can_contain_progression(false);

    // Keep an editable copy of the items distribution from rando options
    _items_distribution = options.items_distribution();

    // Handle progressive items in distribution
    if(options.progressive_items())
        handle_progressive_items(options, game_data);

    if(options.skip_last_dungeon())
    {
        WorldPath* lake_shrine_direct_path = new WorldPath();
        lake_shrine_direct_path->origin(this->node("lake_shrine_exterior"));
        lake_shrine_direct_path->destination(this->node("melzas_fight"));
        _paths.emplace_back(lake_shrine_direct_path);
    }
}

void RandomizerWorld::handle_progressive_items(const RandomizerOptions& options, const GameData& game_data)
{
    const uint16_t SLOT_SWORD = game_data.item(ITEM_SWORD_UPGRADE)->slot_id();
    const uint16_t SLOT_BOW = game_data.item(ITEM_BOW_UPGRADE)->slot_id();
    const uint16_t SLOT_FLAIL = game_data.item(ITEM_FLAIL_UPGRADE)->slot_id();
    const uint16_t SLOT_ARMOR = game_data.item(ITEM_ARMOR_UPGRADE)->slot_id();
    const uint16_t SLOT_BOOTS = game_data.item(ITEM_BOOTS_UPGRADE)->slot_id();
    const uint16_t SLOT_EARTH_MAGIC = game_data.item(ITEM_EARTH_MAGIC_UPGRADE)->slot_id();
    const uint16_t SLOT_WATER_MAGIC = game_data.item(ITEM_WATER_MAGIC_UPGRADE)->slot_id();
    const uint16_t SLOT_FIRE_MAGIC = game_data.item(ITEM_FIRE_MAGIC_UPGRADE)->slot_id();
    const uint16_t SLOT_WIND_MAGIC = game_data.item(ITEM_WIND_MAGIC_UPGRADE)->slot_id();

    std::vector<uint16_t> slots_to_process = {
            SLOT_SWORD, SLOT_BOW, SLOT_FLAIL, SLOT_ARMOR,
            SLOT_EARTH_MAGIC, SLOT_WATER_MAGIC, SLOT_FIRE_MAGIC, SLOT_WIND_MAGIC
    };

    // Boots are only made progressive if their effects are not split
    if(!options.split_boots_effects())
        slots_to_process.emplace_back(SLOT_BOOTS);

    for(uint16_t slot_id : slots_to_process)
    {
        const Item* upgrade_item = nullptr;
        std::vector<const Item*> items_to_convert;
        for(uint8_t item_id = 1 ; item_id < ITEM_COUNT ; ++item_id)
        {
            const Item* item = game_data.item(item_id);
            if(item->slot_id() != slot_id)
                continue;

            if(item->tier() == 64)
            {
                upgrade_item = item;
                continue;
            }

            items_to_convert.emplace_back(item);
        }

        if(!upgrade_item)
            throw RandomizerException("Could not find a valid upgrade item for slot " + std::to_string(slot_id));

        // Update the items distribution to remove specific item tiers and replace them by item upgrades
        for(const Item* item : items_to_convert)
        {
            uint8_t amount = _items_distribution[item->id()];
            _items_distribution[upgrade_item->id()] += amount;
            _items_distribution[item->id()] -= amount;
        }

        // Update logic paths to require a certain count of item upgrades rather than specific items
        for(WorldPath* path : _paths)
        {
            auto& required_items = path->required_items();
            uint8_t highest_tier = 0;
            for(const Item* item : items_to_convert)
            {
                auto it = std::find(required_items.begin(), required_items.end(), item);
                if(it != required_items.end())
                {
                    required_items.erase(it);
                    uint8_t item_tier = item->tier()+1;
                    highest_tier = std::max(highest_tier, item_tier);
                }
            }

            for(int i=1 ; i<=highest_tier ; ++i)
                required_items.emplace_back(upgrade_item);
        }
    }
}