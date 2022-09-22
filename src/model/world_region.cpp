#include "world_region.hpp"

#include <utility>
#include "world_node.hpp"
#include "item_source.hpp"
#include "randomizer_world.hpp"

std::string WorldRegion::hint_name() const
{
    if(_hint_name.empty())
        return std::string("in ") + _name;
    return _hint_name;
}

void WorldRegion::add_node(WorldNode* node)
{
    _nodes.emplace_back(node);
    node->region(this);
}

Json WorldRegion::to_json() const 
{
    Json json;

    json["name"] = _name;
    json["hintName"] = _hint_name;
    json["canBeHintedAsRequired"] = _can_be_hinted_as_required;
    
    json["nodeIds"] = Json::array();
    for(WorldNode* node : _nodes)
        json["nodeIds"].emplace_back(node->id());

    return json;
}

WorldRegion* WorldRegion::from_json(const Json& json, const RandomizerWorld& world)
{
    WorldRegion* region = new WorldRegion();

    for (auto& [key, value] : json.items())
    {
        if (key == "name")
            region->name(value);
        else if (key == "hintName")
            region->hint_name(value);
        else if (key == "canBeHintedAsRequired")
            region->can_be_hinted_as_required(value);
        else if (key == "nodeIds")
        {
            for(const std::string& node_id : value)
                region->add_node(world.node(node_id));
        }
        else
            throw RandomizerException("Unknown key '" + key + "' in WorldRegion JSON");
    }

    return region;
}

std::vector<ItemSource*> WorldRegion::item_sources() const
{
    std::vector<ItemSource*> item_sources;
    for(WorldNode* node : _nodes)
    {
        for(ItemSource* source : node->item_sources())
            item_sources.emplace_back(source);
    }

    return item_sources;
}
