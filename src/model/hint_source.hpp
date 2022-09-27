#pragma once

#include <string>
#include "../tools/json.hpp"

class WorldNode;
class RandomizerWorld;

class HintSource {
private:
    std::string _name;
    WorldNode* _node = nullptr;
    std::string _text;

public:
    HintSource() = default;

    [[nodiscard]] const std::string& name() const { return _name; }
    void name(const std::string& value) { _name = value; }

    [[nodiscard]] WorldNode* node() const { return _node; }
    void node(WorldNode* node) { _node = node; }

    [[nodiscard]] const std::string& text() const { return _text; }
    void text(const std::string& value) { _text = value; }

    [[nodiscard]] virtual Json to_json() const;
    static HintSource* from_json(const Json& json, const RandomizerWorld& world);
};