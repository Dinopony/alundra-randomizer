#pragma once

#include "../game/item.hpp"
#include "../constants/item_codes.hpp"

class GameData;
class WorldNode;

class WorldPath
{
private:
    WorldNode* _from_node;
    WorldNode* _to_node;
    std::vector<const Item*> _required_items;
    std::vector<WorldNode*> _required_nodes;
    uint16_t _weight = 1;

public:
    WorldPath() = default;

    [[nodiscard]] WorldNode* origin() const { return _from_node; }
    void origin(WorldNode* node) { _from_node = node; }

    [[nodiscard]] WorldNode* destination() const { return _to_node; }
    void destination(WorldNode* node) { _to_node = node; }

    [[nodiscard]] const std::vector<const Item*>& required_items() const { return _required_items; }
    [[nodiscard]] std::vector<const Item*>& required_items() { return _required_items; }
    [[nodiscard]] std::map<const Item*, uint16_t> required_items_and_quantity() const;
    void add_required_item(const Item* item) { _required_items.emplace_back(item); }

    [[nodiscard]] const std::vector<WorldNode*>& required_nodes() const { return _required_nodes; }
    [[nodiscard]] bool has_explored_required_nodes(const std::vector<WorldNode*>& explored_nodes) const;

    [[nodiscard]] uint16_t weight() const { return _weight; }
    void weight(uint16_t weight) { _weight = weight; }

    [[nodiscard]] bool is_perfect_opposite_of(WorldPath* other) const;

    [[nodiscard]] Json to_json(bool two_way) const;
    static std::pair<WorldPath*, WorldPath*> from_json(const Json& json, 
                                                       const std::map<std::string, WorldNode*>& nodes, 
                                                       const GameData& game_data);
};
