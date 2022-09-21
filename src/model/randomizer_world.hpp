#pragma once

#include <map>
#include <vector>
#include <array>
#include <string>
#include "../constants/item_codes.hpp"

class GameData;
class RandomizerOptions;
class Item;
class ItemSource;
class WorldNode;
class WorldPath;
class WorldRegion;

class RandomizerWorld {
private:
    /// A list of all item sources (events / locations where to get an item)
    std::vector<ItemSource*> _item_sources;
    
    /// A list of all logic nodes
    std::map<std::string, WorldNode*> _nodes;

    /// A list of all logic paths
    std::vector<WorldPath*> _paths;

    /// A list of all logic regions
    std::vector<WorldRegion*> _regions;

public:
    explicit RandomizerWorld(const RandomizerOptions& options, const GameData& game_data);
    ~RandomizerWorld();

    [[nodiscard]] const std::vector<ItemSource*>& item_sources() const { return _item_sources; }
    [[nodiscard]] std::vector<ItemSource*>& item_sources() { return _item_sources; }
    [[nodiscard]] ItemSource* item_source(const std::string& name) const;
    [[nodiscard]] std::vector<ItemSource*> item_sources_with_item(const Item* item);

    [[nodiscard]] const std::map<std::string, WorldNode*>& nodes() const { return _nodes; }
    [[nodiscard]] WorldNode* node(const std::string& id) const { return _nodes.at(id); }

    [[nodiscard]] const std::vector<WorldPath*>& paths() const { return _paths; }
    void add_path(WorldPath* path) { _paths.emplace_back(path); }
    [[nodiscard]] std::vector<WorldPath*> paths_starting_from_node(const WorldNode* node) const;

    [[nodiscard]] const std::vector<WorldRegion*>& regions() const { return _regions; }
    [[nodiscard]] WorldRegion* region(const std::string& name) const;

    [[nodiscard]] WorldNode* spawn_node() const { return _nodes.at("inoa"); }
    [[nodiscard]] WorldNode* end_node() const { return _nodes.at("end"); }

private:    
    void init_item_sources(const GameData& game_data);
    void init_nodes();
    void init_paths(const GameData& game_data);
    void init_regions();
};
