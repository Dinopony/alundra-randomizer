#include "../game/game_data.hpp"
#include "world_path.hpp"
#include "world_node.hpp"
#include "../tools/vectools.hpp"
#include "../tools/exception.hpp"
#include <utility>

std::map<const Item*, uint16_t> WorldPath::required_items_and_quantity() const
{
    std::map<const Item*, uint16_t> items_and_quantities;
    for (const Item* item : _required_items)
    {
        if (items_and_quantities.contains(item))
            items_and_quantities[item]++;
        else
            items_and_quantities[item] = 1;
    }

    return items_and_quantities;
}

Json WorldPath::to_json() const
{
    Json json;

    json["fromId"] = _from_node->id();
    json["toId"] = _to_node->id();
    if(_two_way)
        json["twoWay"] = _two_way;

    if(_weight > 1)
        json["weight"] = _weight;

    if(!_required_items.empty())
    {
        json["requiredItems"] = Json::array();
        for(const Item* item : _required_items)
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

WorldPath* WorldPath::from_json(const Json& json, const std::map<std::string, WorldNode*>& nodes, const GameData& game_data)
{
    WorldPath* path = new WorldPath();
    bool create_opposite_path = false;

    for(auto& [key, value] : json.items())
    {
        if(key == "fromId")
            path->origin(nodes.at(value));
        else if(key == "toId")
            path->destination(nodes.at(value));
        else if(key == "twoWay")
            path->is_two_way(value);
        else if(key == "requiredNodes")
        {
            for(std::string node_id : value)
                path->_required_nodes.emplace_back(nodes.at(node_id));
        }
        else if(key == "requiredItems")        
        {
            for(const Json& item_json : value)
            {
                const Item* required_item;
                size_t quantity;

                if(item_json.is_object())
                {
                    const std::string& item_name = item_json.at("name");
                    required_item = game_data.item(item_name);
                    quantity = item_json.at("quantity");
                }
                else
                {
                    const std::string& item_name = item_json;
                    required_item = game_data.item(item_name);
                    quantity = 1;
                }

                for(size_t i=0 ; i<quantity ; ++i)
                    path->_required_items.emplace_back(required_item);
            }
        }
        else
            throw RandomizerException("Unknown key '" + key + "' in WorldPath JSON");
    }

    return path;
}
