#pragma once

#include <map>
#include <vector>
#include <string>
#include "../constants/item_codes.hpp"
#include "world.hpp"
#include "item_distribution.hpp"

class WorldNode;
class WorldPath;
class WorldRegion;
class ItemSource;
class RandomizerOptions;

class RandomizerWorld : public World {
private:
    std::vector<ItemSource*> _item_sources;

    std::map<std::string, WorldNode*> _nodes;
    std::map<std::pair<WorldNode*, WorldNode*>, WorldPath*> _paths;
    std::vector<WorldRegion*> _regions;

    std::array<ItemDistribution, ITEM_COUNT> _item_distributions;

public:
    RandomizerWorld() = default;
    ~RandomizerWorld();

    void apply_options(const RandomizerOptions& options);

    [[nodiscard]] std::array<std::string, ITEM_COUNT> item_names() const;

    [[nodiscard]] const std::vector<ItemSource*>& item_sources() const { return _item_sources; }
    [[nodiscard]] std::vector<ItemSource*>& item_sources() { return _item_sources; }
    [[nodiscard]] ItemSource* item_source(const std::string& name) const;
    [[nodiscard]] std::vector<ItemSource*> item_sources_with_item(Item* item);

    [[nodiscard]] const std::map<std::string, WorldNode*>& nodes() const { return _nodes; }
    [[nodiscard]] WorldNode* node(const std::string& id) const { return _nodes.at(id); }

    [[nodiscard]] const std::map<std::pair<WorldNode*, WorldNode*>, WorldPath*>& paths() const { return _paths; }
    WorldPath* path(WorldNode* origin, WorldNode* destination);
    WorldPath* path(const std::string& origin_name, const std::string& destination_name);
    void add_path(WorldPath* path);

    [[nodiscard]] const std::vector<WorldRegion*>& regions() const { return _regions; }
    [[nodiscard]] WorldRegion* region(const std::string& name) const;

    [[nodiscard]] const std::array<ItemDistribution, ITEM_COUNT>& item_distributions() const { return _item_distributions; }
    [[nodiscard]] const ItemDistribution* item_distribution(uint8_t item_id) const { return &_item_distributions[item_id]; }
    [[nodiscard]] ItemDistribution* item_distribution(uint8_t item_id) { return &_item_distributions[item_id]; }
    [[nodiscard]] std::map<uint8_t, uint16_t> item_quantities() const;

    [[nodiscard]] WorldNode* spawn_node() const { return _nodes.at("inoa"); }
    [[nodiscard]] WorldNode* end_node() const { return _nodes.at("end"); }

    void load_model_from_json();

private:
    void load_items();
    void load_item_sources();
    void load_nodes();
    void load_paths();
    void load_regions();
};
