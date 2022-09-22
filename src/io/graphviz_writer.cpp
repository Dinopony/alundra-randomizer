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


void write_logic_as_dot(const RandomizerWorld& world, const std::string& output_path)
{
    std::ofstream graphviz(output_path);
    graphviz << "digraph {\n";

    graphviz << "\tgraph [pad=0.5, nodesep=0.7];\n";
    graphviz << "\tnode[shape=rect];\n";
    graphviz << "\tlayout = neato;\n\n";

    std::map<WorldPath*, bool> _paths_two_way;

    std::vector<WorldPath*> paths = world.paths();
    for(size_t i=0 ; i<paths.size() ; ++i)
    {
        WorldPath* path = paths[i];
        graphviz << "\t" << path->origin()->id() << " -> " << path->destination()->id() << " [";
        if (path->is_two_way())
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
            const char* current_color = COLORS[i % COLORS_SIZE];
            graphviz << "color=" << current_color << " ";
            graphviz << "fontcolor=" << current_color << " ";
            graphviz << "label=\"" << stringtools::join(required_names, "\\n") << "\" ";
        }

        graphviz << "]\n";
    }

    graphviz << "\n";
    uint32_t i=0;

    for(auto& [node_id, node] : world.nodes())
    {
        graphviz << "\t" << node_id << "[";
        // Node position
        if(node->position_in_graph().first != INT_MIN)
            graphviz << " pos=\"" << node->position_in_graph().first << "," << node->position_in_graph().second << "!\"";
        // Node label
        graphviz << " label=\"" << node_id;
        if(!node->item_sources().empty())
            graphviz << "\\n(" << std::to_string(node->item_sources().size()) << " items)";
        graphviz << "\"";
        // Node shape if starting or end node
        if(node == world.spawn_node() || node == world.end_node())
            graphviz << R"( shape=circle style=filled fillcolor="#AAAAFF" fixedsize=true width="2")";
        graphviz << "]\n";
    }

    graphviz << "}\n"; 
}

} // namespace graphviz end
