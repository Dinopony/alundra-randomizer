#include "hint_source.hpp"

#include "world_node.hpp"
#include "randomizer_world.hpp"

Json HintSource::to_json() const
{
    Json json;

    json["name"] = _name;
    if(_node)
        json["nodeId"] = _node->id();
    json["text"] = _text;

    return json;
}

HintSource* HintSource::from_json(const Json& json, const RandomizerWorld& world)
{
    HintSource* source = new HintSource();

    for(auto& [key, value] : json.items())
    {
        if(key == "name")
            source->name(value);
        else if(key == "nodeId")
            source->node(world.node(value));
        else if(key == "text")
            source->text(value);
        else
            throw RandomizerException("Unknown key '" + key + "' in HintSource JSON");
    }

    return source;
}
