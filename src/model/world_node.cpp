#include "world_node.hpp"

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
