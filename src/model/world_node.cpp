#include "world_node.hpp"

void WorldNode::remove_outgoing_path(WorldPath* path)
{
    auto it = std::find(_outgoing_paths.begin(), _outgoing_paths.end(), path);
    _outgoing_paths.erase(it);
}

void WorldNode::remove_ingoing_path(WorldPath* path)
{
    auto it = std::find(_ingoing_paths.begin(), _ingoing_paths.end(), path);
    _ingoing_paths.erase(it);
}

Json WorldNode::to_json() const
{
    Json json;

    if (!_hints.empty())
        json["hints"] = _hints;

    return json;
}

WorldNode* WorldNode::from_json(const std::string &id, const Json &json)
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
