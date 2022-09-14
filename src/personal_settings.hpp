#pragma once

#include <cstdint>
#include <utility>
#include "tools/json.hpp"
#include "constants/item_codes.hpp"

class ArgumentDictionary;

class PersonalSettings
{
private:
    // TODO

public:
    explicit PersonalSettings(const ArgumentDictionary& args);

    void parse_json(const Json& json);
};
