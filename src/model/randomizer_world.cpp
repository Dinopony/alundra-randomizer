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
    this->load_model_from_json(game_data);
}

RandomizerWorld::~RandomizerWorld()
{
    for (ItemSource* source : _item_sources)
        delete source;
    for (auto& [key, node] : _nodes)
        delete node;
    for (auto& [key, path] : _paths)
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

void RandomizerWorld::load_item_sources(const GameData& game_data)
{
    Json item_sources_json = Json::parse(ITEM_SOURCES_JSON);
    for(const Json& source_json : item_sources_json)
    {
        _item_sources.emplace_back(ItemSource::from_json(source_json, game_data));
    }

#ifdef DEBUG
    std::cout << _item_sources.size() << " item sources loaded." << std::endl;
#endif

    // The following chests are absent from the game on release or modded out of the game for the rando, and their IDs are therefore free:
    // 0x0E (14): Mercator Kitchen (variant?)
    // 0x1E (30): King Nole's Cave spiral staircase (variant with enemies?) ---> 29 is the one used in rando
    // 0x20 (32): Boulder chase hallway (variant with enemies?) ---> 31 is the one used in rando
    // 0x25 (37): Thieves Hideout entrance (variant with water)
    // 0x27 (39): Thieves Hideout entrance (waterless variant)
    // 0x28 (40): Thieves Hideout entrance (waterless variant)
    // 0x33 (51): Thieves Hideout second room (waterless variant)
    // 0x3D (61): Thieves Hideout reward room (Kayla cutscene variant)
    // 0x3E (62): Thieves Hideout reward room (Kayla cutscene variant)
    // 0x3F (63): Thieves Hideout reward room (Kayla cutscene variant)
    // 0x40 (64): Thieves Hideout reward room (Kayla cutscene variant)
    // 0x41 (65): Thieves Hideout reward room (Kayla cutscene variant)
    // 0xBB (187): Crypt (Larson. E room)
    // 0xBC (188): Crypt (Larson. E room)
    // 0xBD (189): Crypt (Larson. E room)
    // 0xBE (190): Crypt (Larson. E room)
    // 0xC3 (195): Map 712 / 0x2C8 ???
}

WorldPath* RandomizerWorld::path(WorldNode* origin, WorldNode* destination)
{
    return _paths.at(std::make_pair(origin, destination));
}

WorldPath* RandomizerWorld::path(const std::string& origin_name, const std::string& destination_name)
{
    WorldNode* origin = _nodes.at(origin_name);
    WorldNode* destination = _nodes.at(destination_name);
    return this->path(origin, destination);
}

void RandomizerWorld::add_path(WorldPath* path)
{
    _paths[std::make_pair(path->origin(), path->destination())] = path;
}

WorldRegion* RandomizerWorld::region(const std::string& name) const
{
    for(WorldRegion* region : _regions)
        if(region->name() == name)
            return region;
    return nullptr;
}

void RandomizerWorld::load_model_from_json(const GameData& game_data)
{
    this->load_item_sources(game_data);
    this->load_nodes();
    this->load_paths(game_data);
    this->load_regions();
}

void RandomizerWorld::load_nodes()
{
    Json nodes_json = Json::parse(WORLD_NODES_JSON);
    for(auto& [node_id, node_json] : nodes_json.items())
    {
        WorldNode* new_node = WorldNode::from_json(node_id, node_json);
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

void RandomizerWorld::load_paths(const GameData& game_data)
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

void RandomizerWorld::load_regions()
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
