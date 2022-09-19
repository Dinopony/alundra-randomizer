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
    std::string _id;
    std::vector<ItemSource*> _item_sources;
    std::vector<WorldPath*> _outgoing_paths;
    std::vector<WorldPath*> _ingoing_paths;
    std::vector<std::string> _hints;
    WorldRegion* _region = nullptr;

public:
    WorldNode() = default;

    [[nodiscard]] const std::string& id() const { return _id; }

    [[nodiscard]] const std::vector<ItemSource*>& item_sources() const { return _item_sources; }
    void add_item_source(ItemSource* source) {  _item_sources.emplace_back(source); }

    [[nodiscard]] const std::vector<WorldPath*>& outgoing_paths() const { return _outgoing_paths; }
    void add_outgoing_path(WorldPath* path) { _outgoing_paths.emplace_back(path); }
    void remove_outgoing_path(WorldPath* path);

    [[nodiscard]] const std::vector<WorldPath*>& ingoing_paths() const { return _ingoing_paths; }
    void add_ingoing_path(WorldPath* path) { _ingoing_paths.emplace_back(path); }
    void remove_ingoing_path(WorldPath* path);

    [[nodiscard]] const std::vector<std::string>& hints() const { return _hints; }
    void add_hint(const std::string& hint) { _hints.emplace_back(hint); }

    [[nodiscard]] WorldRegion* region() const { return _region; }
    void region(WorldRegion* region) { _region = region; }

    [[nodiscard]] Json to_json() const;
    static WorldNode* from_json(const std::string& id, const Json& json);  
};
