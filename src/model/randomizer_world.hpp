#pragma once

#include <map>
#include <vector>
#include <string>
#include "../constants/item_codes.hpp"
#include "world.hpp"

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

    std::array<uint8_t, ITEM_COUNT> _item_distribution;

public:
    explicit RandomizerWorld(const RandomizerOptions& options);
    ~RandomizerWorld();

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

    [[nodiscard]] uint8_t item_quantity_in_distribution(uint8_t item_id) const { return _item_distribution[item_id]; }
    [[nodiscard]] std::map<uint8_t, uint8_t> item_quantities_in_distribution() const;

    [[nodiscard]] WorldNode* spawn_node() const { return _nodes.at("inoa"); }
    [[nodiscard]] WorldNode* end_node() const { return _nodes.at("end"); }

private:    
    void load_model_from_json();
    void load_items();
    void load_item_sources();
    void load_nodes();
    void load_paths();
    void load_regions();
};
