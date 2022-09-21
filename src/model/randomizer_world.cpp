#include "randomizer_world.hpp"

#include "../game/game_data.hpp"
#include "world_node.hpp"
#include "world_path.hpp"
#include "world_region.hpp"
#include "../randomizer_options.hpp"

// Include headers automatically generated from model json files
#include "data/item_source.json.hxx"
#include "data/world_node.json.hxx"
#include "data/world_path.json.hxx"
#include "data/world_region.json.hxx"

#include "../tools/exception.hpp"

#include <iostream>

RandomizerWorld::RandomizerWorld(const RandomizerOptions& options, const GameData& game_data)
{
    this->init_item_sources(game_data);
    this->init_nodes();
    this->init_paths(game_data);
    this->init_regions();
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
}

ItemSource* RandomizerWorld::item_source(const std::string& name) const
{
    for (ItemSource* source : _item_sources)
        if (source->name() == name)
            return source;

    throw std::out_of_range("No source with given name");
}

std::vector<ItemSource*> RandomizerWorld::item_sources_with_item(const Item* item)
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
        if(path->origin() == node)
            ret.emplace_back(path);
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
        _item_sources.emplace_back(ItemSource::from_json(source_json, game_data));
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

    for(ItemSource* source : this->item_sources())
    {
        const std::string& node_id = source->node_id();
        try {
            _nodes.at(node_id)->add_item_source(source);
        } catch(std::out_of_range&) {
            throw RandomizerException("Could not find node '" + node_id + "' referenced by item source '" + source->name() + "'");
        }
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
        std::pair<WorldPath*, WorldPath*> paths = WorldPath::from_json(path_json, _nodes, game_data);
        this->add_path(paths.first);
        if(paths.second)
            this->add_path(paths.second);
    }

#ifdef DEBUG
    std::cout << _paths.size() << " paths loaded." << std::endl;
#endif
}

void RandomizerWorld::init_regions()
{
    Json regions_json = Json::parse(WORLD_REGIONS_JSON);
    for(const Json& region_json : regions_json)
        _regions.emplace_back(WorldRegion::from_json(region_json, _nodes));

#ifdef DEBUG
    std::cout << _regions.size() << " regions loaded." << std::endl;
#endif

    for(auto& [id, node] : _nodes)
        if(node->region() == nullptr)
            throw RandomizerException("Node '" + node->id() + "' doesn't belong to any region");
}
