#include "io.hpp"


#include "../model/world.hpp"
#include "../tools/stringtools.hpp"

#include "../model/data/world_path.json.hxx"
#include "../model/world_node.hpp"
#include "../model/world_region.hpp"
#include "../model/randomizer_world.hpp"

namespace GraphvizWriter {

constexpr const char* COLORS[] = { "indianred2", "lightslateblue", "limegreen", "deeppink2", "darkorchid3", "chocolate2", "darkturquoise" };
size_t COLORS_SIZE = 7;


void write_logic_as_dot(const RandomizerWorld& world, const std::string& path)
{
//   const std::map<std::string, std::string> POS_FOR_NODES = {
//           {"overworld_a1", "-2,2"}, {"overworld_b1", "-1,2"}, {"overworld_c1", "0,2"}, {"overworld_d1", "1,2"}, {"overworld_e1", "2,2"},
//           {"overworld_a2", "-2,1"}, {"overworld_b2", "-1,1"}, {"overworld_c2", "0,1"}, {"overworld_d2", "1,1"}, {"overworld_e2", "2,1"},
//           {"overworld_a3", "-2,0"}, {"overworld_b3", "-1,0"}, {"inoa", "0,0"},         {"overworld_d3", "1,0"}, {"overworld_e3", "2,0"},
//           {"overworld_a4_middle", "-2,-1"}, {"overworld_b4", "-1,-1"}, {"overworld_c4", "0,-1"}, {"overworld_d4", "1,-1"}, {"overworld_e4", "2,-1"},
//   };

    std::ofstream graphviz(path);
    graphviz << "digraph {\n";

    graphviz << "\tgraph [pad=0.5, nodesep=0.5, ranksep=0.3];\n";
    graphviz << "\tnode[shape=rect];\n\n";

    Json paths_json = Json::parse(WORLD_PATHS_JSON);
    uint32_t path_i = 0;
    for(const Json& json : paths_json)
    {
        WorldNode* from = world.node(json["fromId"]);
        WorldNode* to = world.node(json["toId"]);
        graphviz << "\t" << from->id() << " -> " << to->id() << " [";
        if(json.contains("twoWay") && json.at("twoWay"))
            graphviz << "dir=both ";

        std::vector<std::string> required_names;
        if(json.contains("requiredItems"))
            for(const std::string item_name : json.at("requiredItems"))
                required_names.emplace_back(item_name);
            
        if(json.contains("requiredNodes"))
            for(const std::string node_id : json.at("requiredNodes"))
                required_names.emplace_back("Access to " + world.node(node_id)->name());

        if(!required_names.empty())
        {
            const char* current_color = COLORS[path_i % COLORS_SIZE];
            graphviz << "color=" << current_color << " ";
            graphviz << "fontcolor=" << current_color << " ";
            graphviz << "label=\"" << stringtools::join(required_names, "\\n") << "\" ";
        }

        graphviz << "]\n";
        path_i++;
    }

    graphviz << "\n";
    uint32_t i=0;
    for(WorldRegion* region : world.regions())
    {
        graphviz << "\tsubgraph cluster_" << i++ << " {\n";
        graphviz << "\t\tperipheries=0\n";

        for(WorldNode* node : region->nodes())
        {
            graphviz << "\t\t" << node->id() << "[";
            graphviz << " label=\"" << node->id() << "\\n(" << std::to_string(node->item_sources().size()) << " items)\"";
//            if(POS_FOR_NODES.count(node->id()))
//                graphviz << " pos=\"" << POS_FOR_NODES.at(node->id()) << "!\"";

            graphviz << "]\n";
        }

        graphviz << "\t}\n\n";
    }

    graphviz << "}\n"; 
}

} // namespace graphviz end
