#pragma once

#include <cstdint>
#include <utility>
#include "tools/json.hpp"
#include "constants/item_codes.hpp"

class ArgumentDictionary;

class PersonalSettings
{
private:
    bool _remove_music = false;

public:
    explicit PersonalSettings(const ArgumentDictionary& args);

    [[nodiscard]] bool remove_music() const { return _remove_music; }

private:
    void parse_json(const Json& json);
};
