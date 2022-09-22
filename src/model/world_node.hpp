#pragma once

#include <utility>
#include <vector>

#include "item_source.hpp"
#include "../tools/exception.hpp"

class WorldRegion;
class WorldPath;

class WorldNode
{
private:
    /// A unique identifier for the node
    std::string _id;
    
    /// A list of all item sources contained inside the node
    std::vector<ItemSource*> _item_sources;
    
    /// A list of hints that can be given to point to an item source inside this node
    std::vector<std::string> _hints;

    /// The region this node belongs to
    WorldRegion* _region = nullptr;

public:
    WorldNode() = default;

    [[nodiscard]] const std::string& id() const { return _id; }

    [[nodiscard]] const std::vector<ItemSource*>& item_sources() const { return _item_sources; }
    void add_item_source(ItemSource* source) { _item_sources.emplace_back(source); }
    void remove_item_source(ItemSource* source);

    [[nodiscard]] const std::vector<std::string>& hints() const { return _hints; }
    void add_hint(const std::string& hint) { _hints.emplace_back(hint); }

    [[nodiscard]] WorldRegion* region() const { return _region; }
    void region(WorldRegion* region) { _region = region; }

    [[nodiscard]] Json to_json() const;
    static WorldNode* from_json(const Json& json, const std::string& id);
};
