#pragma once

#include <string>
#include <vector>
#include <map>
#include "../tools/json.hpp"

class WorldNode;
class ItemSource;
class RandomizerWorld;

class WorldRegion
{
private:
    /// The name of the region (as seen in the spoiler log)
    std::string _name;

    /// The nodes contained inside that region
    std::vector<WorldNode*> _nodes;

    /// The name of the region (as described in hints)
    std::string _hint_name;

    /// If true, a hint can say if exploring this region is necessary to complete the seed
    bool _can_be_hinted_as_required = false;

public:
    WorldRegion() = default;

    [[nodiscard]] const std::string& name() const { return _name; }
    void name(const std::string& value) { _name = value; }

    [[nodiscard]] const std::vector<WorldNode*>& nodes() const { return _nodes; }
    void add_node(WorldNode* node);

    [[nodiscard]] std::string hint_name() const;
    void hint_name(const std::string& value) { _hint_name = value; }

    [[nodiscard]] bool can_be_hinted_as_required() const { return _can_be_hinted_as_required; }
    void can_be_hinted_as_required(bool value) { _can_be_hinted_as_required = value; }

    [[nodiscard]] std::vector<ItemSource*> item_sources() const;

    [[nodiscard]] Json to_json() const;
    static WorldRegion* from_json(const Json& json, const RandomizerWorld& world);
};
