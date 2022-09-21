#include "io.hpp"

#include "../tools/stringtools.hpp"

#include "../model/world_node.hpp"
#include "../model/world_region.hpp"
#include "../model/world_path.hpp"
#include "../model/randomizer_world.hpp"

void ModelWriter::write_logic_model(const RandomizerWorld& world)
{
    Json nodes_json;
    for(auto& [id, node] : world.nodes())
        nodes_json[id] = node->to_json();
    dump_json_to_file(nodes_json, "./json_data/world_node.json");

    Json paths_json = Json::array();
    for(WorldPath* path : world.paths())
        paths_json.emplace_back(path->to_json());
    dump_json_to_file(paths_json, "./json_data/world_path.json");

    Json item_sources_json = Json::array();
    for(ItemSource* source : world.item_sources())
        item_sources_json.emplace_back(source->to_json());
    dump_json_to_file(item_sources_json, "./json_data/item_sources.json");

    Json regions_json = Json::array();
    for(WorldRegion* region : world.regions())
        regions_json.emplace_back(region->to_json());
    dump_json_to_file(regions_json, "./json_data/world_region.json");
}
