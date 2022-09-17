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
    std::ofstream graphviz(path);
    graphviz << "digraph {\n";

    graphviz << "\tgraph [pad=0.5, nodesep=1, ranksep=1.5];\n";
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
            graphviz << " label=\"" << node->id();
            if(!node->item_sources().empty())
                graphviz << "\\n(" << std::to_string(node->item_sources().size()) << " items)";
            graphviz << "\"]\n";
        }

        graphviz << "\t}\n\n";
    }

    graphviz << "}\n"; 
}

} // namespace graphviz end
