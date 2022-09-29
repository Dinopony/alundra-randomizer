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

    // Handle progressive items in distribution
    _items_distribution = options.items_distribution();
    if(options.progressive_items())
    {
        auto convert_item_in_distrib = [this](const std::vector<uint8_t>& items_from, uint8_t item_to) {
            for(uint8_t item_from : items_from)
            {
                _items_distribution[item_to] += _items_distribution[item_from];
                _items_distribution[item_from] = 0;
            }
        };

        convert_item_in_distrib({ ITEM_DAGGER, ITEM_FIEND_BLADE, ITEM_HOLY_SWORD, ITEM_LEGEND_SWORD }, ITEM_SWORD);
        convert_item_in_distrib({ ITEM_WILLOW_BOW },                                                   ITEM_HUNTERS_BOW);
        convert_item_in_distrib({ ITEM_STEEL_FLAIL },                                                  ITEM_IRON_FLAIL);
        convert_item_in_distrib({ ITEM_CLOTH_ARMOR, ITEM_ANCIENT_ARMOR, ITEM_SILVER_ARMOR},            ITEM_LEATHER_ARMOR);
        convert_item_in_distrib({ ITEM_EARTH_BOOK },                                                   ITEM_EARTH_SCROLL);
        convert_item_in_distrib({ ITEM_WATER_BOOK },                                                   ITEM_WATER_SCROLL);
        convert_item_in_distrib({ ITEM_FIRE_BOOK },                                                    ITEM_FIRE_SCROLL);
        convert_item_in_distrib({ ITEM_WIND_BOOK },                                                    ITEM_WIND_SCROLL);

        // Boots are only made progressive if their effects are not split
        if(!options.split_boots_effects())
        {
            convert_item_in_distrib({ ITEM_SHORT_BOOTS, ITEM_MERMAN_BOOTS, ITEM_CHARM_BOOTS}, ITEM_LONG_BOOTS);

            for(WorldPath* path : _paths)
            {
                auto& required_items = path->required_items();
                uint8_t highest_tier = 0;
                if(vectools::contains(required_items, game_data.item(ITEM_LONG_BOOTS)))
                {
                    highest_tier = 2;
                    vectools::erase_first(required_items, game_data.item(ITEM_LONG_BOOTS));
                }
                if(vectools::contains(required_items, game_data.item(ITEM_MERMAN_BOOTS)))
                {
                    highest_tier = 3;
                    vectools::erase_first(required_items, game_data.item(ITEM_MERMAN_BOOTS));
                }
                if(vectools::contains(required_items, game_data.item(ITEM_CHARM_BOOTS)))
                {
                    highest_tier = 4;
                    vectools::erase_first(required_items, game_data.item(ITEM_CHARM_BOOTS));
                }

                for(int i=1 ; i<highest_tier ; ++i)
                    required_items.emplace_back(game_data.item(ITEM_LONG_BOOTS));
            }
        }
    }
}
