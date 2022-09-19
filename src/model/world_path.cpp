#include "world_path.hpp"
#include "world_node.hpp"
#include "../tools/vectools.hpp"
#include "../tools/exception.hpp"
#include <utility>

WorldPath::WorldPath(WorldNode* from_node, WorldNode* to_node, uint16_t weight, std::vector<Item*> required_items,
                    std::vector<WorldNode*> required_nodes, std::vector<Item*> items_placed_when_crossing) :
    _from_node                  (from_node),
    _to_node                    (to_node),
    _weight                     (weight),
    _required_items             (std::move(required_items)),
    _required_nodes             (std::move(required_nodes))
{
    _from_node->add_outgoing_path(this);
    _to_node->add_ingoing_path(this);
}

bool WorldPath::has_explored_required_nodes(const std::vector<WorldNode*>& explored_nodes) const
{
    for(WorldNode* node : _required_nodes)
        if(!vectools::contains(explored_nodes, node))
            return false;
    return true;
}

bool WorldPath::is_perfect_opposite_of(WorldPath* other) const
{
    if(_from_node != other->_to_node)
        return false;
    if(_to_node != other->_from_node)
        return false;
    if(_weight != other->_weight)
        return false;
    if(_required_items != other->_required_items)
        return false;
    if(_required_nodes != other->_required_nodes)
        return false;

    return true;
}

Json WorldPath::to_json(bool two_way) const
{
    Json json;

    json["fromId"] = _from_node->id();
    json["toId"] = _to_node->id();
    json["twoWay"] = two_way;

    if(_weight > 1)
        json["weight"] = _weight;

    if(!_required_items.empty())
    {
        json["requiredItems"] = Json::array();
        for(Item* item : _required_items)
            json["requiredItems"].emplace_back(item->name());
    }

    if(!_required_nodes.empty())
    {
        json["requiredNodes"] = Json::array();
        for(WorldNode* node : _required_nodes)
            json["requiredNodes"].emplace_back(node->id());
    }

    return json;
}

static Item* find_item_from_name(const std::vector<Item*>& items, const std::string& name)
{
    for(Item* item : items)
        if(item && item->name() == name)
            return item;

    throw RandomizerException("Could not find item with name '" + name + "' from required items in world paths JSON.");
}

WorldPath* WorldPath::from_json(const Json& json, const std::map<std::string, WorldNode*>& nodes, const std::vector<Item*>& items)
{
    const std::string& from_id = json.at("fromId");
    WorldNode* from_node = nodes.at(from_id);

    const std::string& to_id = json.at("toId");
    WorldNode* to_node = nodes.at(to_id);

    uint16_t weight = json.value("weight", 1);

    std::vector<Item*> required_items;
    if(json.contains("requiredItems"))
        for(const std::string& item_name : json.at("requiredItems"))
            required_items.emplace_back(find_item_from_name(items, item_name));

    std::vector<WorldNode*> required_nodes;
    if(json.contains("requiredNodes"))
        for(const std::string& node_id : json.at("requiredNodes"))
            required_nodes.emplace_back(nodes.at(node_id));
        
    return new WorldPath(from_node, to_node, weight, required_items, required_nodes);
}
