#include "world_path.hpp"
#include "world_node.hpp"
#include "../tools/vectools.hpp"
#include "../tools/exception.hpp"
#include <utility>

void WorldPath::origin(WorldNode* node)
{
    if(_from_node)
        _from_node->remove_outgoing_path(this);
    _from_node = node;
    node->add_outgoing_path(this);
}

void WorldPath::destination(WorldNode* node)
{
    if(_to_node)
        _to_node->remove_ingoing_path(this);
    _to_node = node;
    node->add_ingoing_path(this);
}

std::map<Item*, uint16_t> WorldPath::required_items_and_quantity() const
{
    std::map<Item*, uint16_t> items_and_quantities;
    for (Item* item : _required_items)
    {
        if (items_and_quantities.contains(item))
            items_and_quantities[item]++;
        else
            items_and_quantities[item] = 1;
    }

    return items_and_quantities;
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

std::pair<WorldPath*, WorldPath*> WorldPath::from_json(const Json& json, const std::map<std::string, WorldNode*>& nodes, const std::vector<Item*>& items)
{
    WorldPath* path = new WorldPath();
    bool create_opposite_path = false;

    for(auto& [key, value] : json.items())
    {
        if(key == "fromId")
            path->origin(nodes.at(value));
        else if(key == "toId")
            path->destination(nodes.at(value));
        else if(key == "requiredNodes")
        {
            for(const std::string& node_id : value)
                path->_required_nodes.emplace_back(nodes.at(node_id));
        }
        else if(key == "requiredItems")        
        {
            for(const Json& item_json : value)
            {
                Item* required_item;
                size_t quantity;

                if(item_json.is_object())
                {
                    required_item = find_item_from_name(items, item_json.at("name"));
                    quantity = item_json.at("quantity");
                }
                else
                {
                    required_item = find_item_from_name(items, item_json);
                    quantity = 1;
                }

                for(size_t i=0 ; i<quantity ; ++i)
                    path->_required_items.emplace_back(required_item);
            }
        }
        else if(key == "twoWay")
            create_opposite_path = value;
        else
            throw RandomizerException("Unknown key '" + key + "' in WorldPath JSON");
    }

    WorldPath* path_opposite = nullptr;
    if(create_opposite_path)
    {
        path_opposite = new WorldPath();
        path_opposite->origin(path->destination());
        path_opposite->destination(path->origin());
        path_opposite->_required_items = path->_required_items;
        path_opposite->_required_nodes = path->_required_nodes;
        path_opposite->_weight = path->_weight;
    }

    return std::make_pair(path, path_opposite);
}
