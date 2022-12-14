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

    /// If true, the item sources in this node shouldn't contain two instances of the same item. This will not be
    /// strictly enforced, and might be bypassed if there is no other way to finish distributing the items
    bool _should_have_unique_items = false;

    /// The position of the node when exported to a visual graph
    std::pair<int, int> _position_in_graph;

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

    [[nodiscard]] bool should_have_unique_items() const { return _should_have_unique_items; }
    void should_have_unique_items(bool value) { _should_have_unique_items = value; }

    [[nodiscard]] const std::pair<int, int>& position_in_graph() const { return _position_in_graph; }

    [[nodiscard]] Json to_json() const;
    static WorldNode* from_json(const Json& json, const std::string& id);
};
