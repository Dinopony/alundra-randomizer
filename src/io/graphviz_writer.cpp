#include "io.hpp"

#include "../tools/stringtools.hpp"

#include "../model/data/world_path.json.hxx"
#include "../model/world_node.hpp"
#include "../model/world_path.hpp"
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

    std::map<WorldPath*, bool> _paths_two_way;

    Json paths_json = Json::parse(WORLD_PATHS_JSON);
    uint32_t path_i = 0;
    for(auto& [nodes_pair, path] : world.paths())
    {
        bool found_opposite = false;
        for (auto& [path_2, two_way] : _paths_two_way)
        {
            if (path->is_perfect_opposite_of(path_2))
            {
                two_way = true;
                found_opposite = true;
                break;
            }
        }

        if (!found_opposite)
            _paths_two_way[path] = false;
    }

    for (auto& [path, two_way] : _paths_two_way)
    {
        graphviz << "\t" << path->origin()->id() << " -> " << path->destination()->id() << " [";
        if (two_way)
            graphviz << "dir=both ";

        std::vector<std::string> required_names;
        if (!path->required_items().empty())
        {
            for (auto& [item, qty] : path->required_items_and_quantity())
            {
                std::string item_descriptor = item->name();
                if (qty > 1)
                    item_descriptor += " x" + std::to_string(qty);
                required_names.emplace_back(item_descriptor);
            }
        }

        for (WorldNode* node : path->required_nodes())
            required_names.emplace_back("Access to " + node->id());

        if (!required_names.empty())
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
