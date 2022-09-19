#include "item_source.hpp"

#include <utility>
#include "world.hpp"
#include "../tools/exception.hpp"
#include "../tools/json_tools.hpp"

Json ItemSource::to_json() const
{
    Json json;

    json["name"] = _name;
    json["nodeId"] = _node_id;
    if(!_addresses.empty())
        json["address"] = addresses_to_json(_addresses);
    if(!_sprite_addresses.empty())
        json["spriteAddress"] = addresses_to_json(_sprite_addresses);
    if(_merrick_item_address)
        json["merrickItemAddress"] = _merrick_item_address;
    if(!_can_contain_progression)
        json["canContainProgression"] = _can_contain_progression;
    if(!_hints.empty())
        json["hints"] = _hints;
    if(_vanilla_item)
        json["vanillaItem"] = _vanilla_item->name();

    return json;
}

ItemSource* ItemSource::from_json(const Json& json, const World& world)
{
    ItemSource* source = new ItemSource();

    for(auto& [key, value] : json.items())
    {
        if(key == "name")                        
            source->_name = value;
        else if(key == "nodeId")
            source->_node_id = value;
        else if(key == "address")
            source->_addresses = parse_addresses_from_json(value);
        else if(key == "spriteAddress")
            source->_sprite_addresses = parse_addresses_from_json(value);
        else if(key == "merrickItemAddress")
            source->_merrick_item_address = parse_addresses_from_json(value)[0];
        else if(key == "hints")
            value.get_to(source->_hints);
        else if(key == "canContainProgression")
            source->_can_contain_progression = value;
        else if(key == "vanillaItem")
            source->_vanilla_item = world.item(std::string(value));
        else
            throw RandomizerException("Unknown key '" + key + "' in ItemSource JSON");
    }

    return source;
}
