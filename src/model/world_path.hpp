#pragma once

#include "item.hpp"
#include "../constants/item_codes.hpp"

class WorldNode;

class WorldPath
{
private:
    WorldNode* _from_node;
    WorldNode* _to_node;
    uint16_t _weight;
    std::vector<Item*> _required_items; // TODO: Handle AND / OR item groups
    std::vector<WorldNode*> _required_nodes;
    
public:
    WorldPath(WorldNode* from_node, 
                WorldNode* to_node, 
                uint16_t weight = 1, 
                std::vector<Item*> required_items = {},
                std::vector<WorldNode*> required_nodes = {},
                std::vector<Item*> items_placed_when_crossing = {});

    [[nodiscard]] const std::vector<Item*>& required_items() const { return _required_items; }
    [[nodiscard]] std::vector<Item*>& required_items() { return _required_items; }
    void add_required_item(Item* item) { _required_items.emplace_back(item); }

    [[nodiscard]] WorldNode* origin() const { return _from_node; }
    [[nodiscard]] WorldNode* destination() const { return _to_node; }

    [[nodiscard]] const std::vector<WorldNode*>& required_nodes() const { return _required_nodes; }
    [[nodiscard]] bool has_explored_required_nodes(const std::vector<WorldNode*>& explored_nodes) const;

    [[nodiscard]] uint16_t weight() const { return _weight; }
    void weight(uint16_t weight) { _weight = weight; }

    [[nodiscard]] bool is_perfect_opposite_of(WorldPath* other) const;

    [[nodiscard]] Json to_json(bool two_way) const;
    static WorldPath* from_json(const Json& json, const std::map<std::string, WorldNode*>& nodes, const std::vector<Item*>& items);
};
