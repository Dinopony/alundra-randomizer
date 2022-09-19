#pragma once

#include "json.hpp"
#include <sstream>

inline Json addresses_to_json(const std::vector<uint32_t>& addresses)
{
    std::vector<std::string> addresses_as_str;
    for(uint32_t addr : addresses)
    {
        std::string str;
        std::stringstream ss;
        ss << addr;
        ss >> std::hex >> str;
        addresses_as_str.emplace_back(str);
    }

    if(addresses.size() == 1)
        return Json(addresses_as_str[0]);
    return Json(addresses_as_str);
}

inline std::vector<uint32_t> parse_addresses_from_json(const Json& json)
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