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
class HintSource;

class RandomizerWorld {
private:
    /// A list of all item sources (events / locations where to get an item)
    std::vector<ItemSource*> _item_sources;
    
    /// A list of all logic nodes
    std::map<std::string, WorldNode*> _nodes;

    /// A list of all logic paths, which connect nodes to form an explorable graph
    std::vector<WorldPath*> _paths;

    /**
     * A list of all logic regions, used to group nodes into coherent sets that represent
     * a concrete location inside the game. Mostly used to group item sources in categories
     * inside the spoiler log.
     */
    std::vector<WorldRegion*> _regions;

    /// A dictionary of hints
    std::map<std::string, HintSource*> _hint_sources;

    /**
     * An editable copy of the items distribution from RandomizerOptions.
     * Contains the number of each item type to be scattered inside the world during randomization.
     */
    std::array<uint8_t, ITEM_COUNT> _items_distribution {};

public:
    explicit RandomizerWorld(const GameData& game_data);
    ~RandomizerWorld();

    void apply_options(const RandomizerOptions& options, const GameData& game_data);

    [[nodiscard]] const std::vector<ItemSource*>& item_sources() const { return _item_sources; }
    [[nodiscard]] std::vector<ItemSource*>& item_sources() { return _item_sources; }
    [[nodiscard]] ItemSource* item_source(const std::string& name) const;
    [[nodiscard]] ItemSource* item_source(uint16_t id) const { return _item_sources.at(id); }
    [[nodiscard]] uint16_t item_source_id(const ItemSource* source) const;
    [[nodiscard]] std::vector<ItemSource*> item_sources_with_item(const Item* item) const;

    [[nodiscard]] const std::map<std::string, WorldNode*>& nodes() const { return _nodes; }
    [[nodiscard]] WorldNode* node(const std::string& id) const { return _nodes.at(id); }
    [[nodiscard]] WorldNode* spawn_node() const { return _nodes.at("inoa"); }
    [[nodiscard]] WorldNode* end_node() const { return _nodes.at("end"); }

    [[nodiscard]] const std::vector<WorldPath*>& paths() const { return _paths; }
    void add_path(WorldPath* path) { _paths.emplace_back(path); }
    [[nodiscard]] std::vector<WorldPath*> paths_starting_from_node(const WorldNode* node) const;

    [[nodiscard]] const std::vector<WorldRegion*>& regions() const { return _regions; }
    [[nodiscard]] WorldRegion* region(const std::string& name) const;

    [[nodiscard]] std::map<std::string, HintSource*>& hint_sources() { return _hint_sources; }
    [[nodiscard]] const std::map<std::string, HintSource*>& hint_sources() const { return _hint_sources; }
    [[nodiscard]] const HintSource* hint_source(const std::string& hint_source_id) const { return _hint_sources.at(hint_source_id); }
    [[nodiscard]] HintSource* hint_source(const std::string& hint_source_id) { return _hint_sources.at(hint_source_id); }

    [[nodiscard]] const std::array<uint8_t, ITEM_COUNT>& items_distribution() const { return _items_distribution; }

    [[nodiscard]] std::vector<const Item*> starting_inventory() const { return {}; }

private:    
    void init_item_sources(const GameData& game_data);
    void init_nodes();
    void init_paths(const GameData& game_data);
    void init_regions();
    void init_hint_sources();

    void handle_crests(const RandomizerOptions& options, const GameData& game_data);
    void handle_progressive_items(const RandomizerOptions& options, const GameData& game_data);
};
