#pragma once

#include <string>
#include <vector>
#include <map>
#include "../tools/json.hpp"

class WorldNode;
class ItemSource;

class WorldRegion
{
private:
    std::string _name;
    std::string _hint_name;
    std::vector<WorldNode*> _nodes;
    bool _can_be_hinted_as_required;

public:
    WorldRegion(std::string name, std::string hint_name, 
                std::vector<WorldNode*> nodes, bool can_be_hinted_as_required);

    [[nodiscard]] const std::string& name() const { return _name; }
    [[nodiscard]] const std::string& hint_name() const { return _hint_name; }
    [[nodiscard]] const std::vector<WorldNode*>& nodes() const { return _nodes; }
    [[nodiscard]] bool can_be_hinted_as_required() const { return _can_be_hinted_as_required; }
    [[nodiscard]] std::vector<ItemSource*> item_sources() const;

    [[nodiscard]] Json to_json() const;
    static WorldRegion* from_json(const Json& json, const std::map<std::string, WorldNode*>& all_nodes);
};
