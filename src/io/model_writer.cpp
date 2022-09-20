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

    auto paths_copy = world.paths();
    Json paths_json = Json::array();
    while(!paths_copy.empty())
    {
        auto it = paths_copy.begin();
        std::pair<WorldNode*, WorldNode*> node_pair = it->first;
        WorldPath* path = it->second;
        paths_copy.erase(it);

        bool two_way = false;
        std::pair<WorldNode*, WorldNode*> reverse_pair = std::make_pair(node_pair.second, node_pair.first);
        if(paths_copy.count(reverse_pair))
        {
            WorldPath* reverse_path = paths_copy.at(reverse_pair);
            if(path->is_perfect_opposite_of(reverse_path))
            {
                two_way = true;
                paths_copy.erase(reverse_pair);
            }
        }
        
        paths_json.emplace_back(path->to_json(two_way));
    }
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
