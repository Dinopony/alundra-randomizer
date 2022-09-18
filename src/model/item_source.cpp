#include "item_source.hpp"

#include <utility>
#include "world.hpp"
#include "../tools/exception.hpp"

Json ItemSource::to_json() const
{
    Json json;
    json["name"] = _name;
    json["nodeId"] = _node_id;

    std::vector<std::string> addresses_as_str;
    for(uint32_t addr : _addresses)
    {
        std::string str;
        std::stringstream ss;
        ss << addr;
        ss >> std::hex >> str;
        addresses_as_str.emplace_back(str);
    }

    if(addresses_as_str.size() > 1)
        json["address"] = addresses_as_str;
    else
        json["address"] = addresses_as_str[0];

    std::vector<std::string> sprite_addresses_as_str;
    for(uint32_t addr : _sprite_addresses)
    {
        std::string str;
        std::stringstream ss;
        ss << addr;
        ss >> std::hex >> str;
        sprite_addresses_as_str.emplace_back(str);
    }

    if(sprite_addresses_as_str.size() > 1)
        json["spriteAddress"] = sprite_addresses_as_str;
    else
        json["spriteAddress"] = sprite_addresses_as_str[0];

    if(!_hints.empty())
        json["hints"] = _hints;
    return json;
}

static std::vector<uint32_t> parse_addresses_from_json(const Json& json)
{
    std::vector<std::string> addresses_as_str;
    if(json.is_array())
        json.get_to(addresses_as_str);
    else
        addresses_as_str.emplace_back(json);

    std::vector<uint32_t> addresses;
    for(const std::string& str : addresses_as_str)
    {
        uint32_t addr;
        std::stringstream ss;
        ss << std::hex << str;
        ss >> addr;
        addresses.emplace_back(addr);
    }
    return addresses;
}

ItemSource* ItemSource::from_json(const Json& json, const World& world)
{
    ItemSource* source = new ItemSource();

    source->name(json.at("name"));
    source->node_id(json.at("nodeId"));

    // Addresses
    if(json.contains("address"))
    {
        std::vector<uint32_t> addresses = parse_addresses_from_json(json.at("address"));
        for(uint32_t addr : addresses)
            source->add_address(addr);
    }

    if(json.contains("spriteAddress"))
    {
        std::vector<uint32_t> sprite_addresses = parse_addresses_from_json(json.at("spriteAddress"));
        for(uint32_t addr : sprite_addresses)
            source->add_sprite_address(addr);
    }

    // Hints
    std::vector<std::string> hints;
    if(json.contains("hints")) 
        json.at("hints").get_to(hints);
    for(const std::string& hint : hints)
        source->add_hint(hint);

    // Merrick item address
    if(json.contains("merrickItemAddress"))
    {
        std::string addr_str = json.at("merrickItemAddress");
        uint32_t addr;
        std::stringstream ss;
        ss << std::hex << addr_str;
        ss >> addr;
        source->merrick_item_address(addr);
    }

    return source;
}
