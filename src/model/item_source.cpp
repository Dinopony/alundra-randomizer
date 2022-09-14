#include "item_source.hpp"

#include <utility>

#include <utility>
#include "world.hpp"
#include "../tools/exception.hpp"

ItemSource::ItemSource(std::string  name, std::vector<uint32_t> addresses, const std::string& node_id, const std::vector<std::string>& hints) :
    _name       (std::move(name)),
    _addresses  (std::move(addresses)),
    _node_id    (node_id),
    _hints      (hints)
{}

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

    if(!_hints.empty())
        json["hints"] = _hints;
    return json;
}

ItemSource* ItemSource::from_json(const Json& json, const World& world)
{
    const std::string& name = json.at("name");
    const std::string& node_id = json.at("nodeId");

    std::vector<std::string> addresses_as_str;
    if(json.at("address").is_array())
        json.at("address").get_to(addresses_as_str);
    else
        addresses_as_str.emplace_back(json.at("address"));

    std::vector<uint32_t> addresses;
    for(const std::string& str : addresses_as_str)
    {
        uint32_t addr;
        std::stringstream ss;
        ss << std::hex << str;
        ss >> addr;
        addresses.emplace_back(addr);
    }

    std::vector<std::string> hints;
    if(json.contains("hints")) 
        json.at("hints").get_to(hints);

    return new ItemSource(name, addresses, node_id, hints);
}
