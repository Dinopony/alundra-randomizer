#include "world_region.hpp"

#include <utility>
#include "world_node.hpp"
#include "item_source.hpp"

WorldRegion::WorldRegion(std::string name, std::string hint_name, 
                         std::vector<WorldNode*> nodes, bool can_be_hinted_as_required) :
    _name                       (std::move(name)),
    _hint_name                  (std::move(hint_name)),
    _nodes                      (std::move(nodes)),
    _can_be_hinted_as_required  (can_be_hinted_as_required)
{
    for(WorldNode* node : _nodes)
        node->region(this);
}

Json WorldRegion::to_json() const 
{
    Json json;

    json["name"] = _name;
    json["hintName"] = _hint_name;
    
    json["nodeIds"] = Json::array();
    for(WorldNode* node : _nodes)
        json["nodeIds"].emplace_back(node->id());

    return json;
}

WorldRegion* WorldRegion::from_json(const Json& json, const std::map<std::string, WorldNode*>& all_nodes)
{
    std::string name = json.at("name");

    std::string hint_name = json.value("hintName", "");
    if(hint_name.empty())
        hint_name = "in " + name;

    bool can_be_hinted_as_required = json.value("canBeHintedAsRequired", true);

    std::vector<WorldNode*> nodes;
    for(std::string node_id : json.at("nodeIds"))
        nodes.emplace_back(all_nodes.at(node_id));

}

std::map<std::string, ItemSource*> WorldRegion::item_sources() const
{
    std::map<std::string, ItemSource*> item_sources;
    for(WorldNode* node : _nodes)
    {
        for(ItemSource *source : node->item_sources())
            item_sources[source->pretty_name()] = source;
    }

    return item_sources;
    return new WorldRegion(name, hint_name, nodes, can_be_hinted_as_required);
}
