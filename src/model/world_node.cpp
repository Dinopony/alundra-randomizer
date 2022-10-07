#include "world_node.hpp"

#include <climits>

void WorldNode::remove_item_source(ItemSource* source)
{
    auto it = std::find(_item_sources.begin(), _item_sources.end(), source);
    if(it == _item_sources.end())
        throw RandomizerException("Could not remove item source from node '" + _id + "' because it was not contained inside.");

    _item_sources.erase(it);
}

Json WorldNode::to_json() const
{
    Json json;

    if (!_hints.empty())
        json["hints"] = _hints;

    json["shouldHaveUniqueItems"] = _should_have_unique_items;
    json["graphPos"] = _position_in_graph;

    return json;
}

WorldNode* WorldNode::from_json(const Json& json, const std::string& id)
{
    WorldNode* node = new WorldNode();

    node->_id = id;
    node->_position_in_graph = std::make_pair(INT_MIN, INT_MIN);

    for (auto& [key, value] : json.items())
    {
        if (key == "hints")
            value.get_to(node->_hints);
        else if (key == "graphPos")
            value.get_to(node->_position_in_graph);
        else if (key == "shouldHaveUniqueItems")
            node->should_have_unique_items(value);
        else
            throw RandomizerException("Unknown key '" + key + "' in WorldNode JSON");
    }

    return node;
}
