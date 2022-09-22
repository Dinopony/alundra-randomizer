#include "world_node.hpp"

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

    return json;
}

WorldNode* WorldNode::from_json(const Json &json, const std::string &id)
{
    WorldNode* node = new WorldNode();

    node->_id = id;

    for (auto& [key, value] : json.items())
    {
        if (key == "hints")
            value.get_to(node->_hints);
        else
            throw RandomizerException("Unknown key '" + key + "' in WorldNode JSON");
    }

    return node;
}
