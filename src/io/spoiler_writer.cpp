#include "io.hpp"

#include "../tools/json.hpp"

#include "../model/world_node.hpp"
#include "../model/world_region.hpp"
#include "../model/randomizer_world.hpp"
#include "../randomizer_options.hpp"

Json SpoilerWriter::build_spoiler_json(const RandomizerWorld& world, const RandomizerOptions& options)
{
    Json json;

    // Export item sources
    for(WorldRegion* region : world.regions())
    {
        for(WorldNode* node : region->nodes())
        {
            for(ItemSource* source : node->item_sources())
            {
                Item* item = source->item();
                json["itemSources"][region->name()][source->pretty_name()] = item->name();
            }
        }
    }

    return json;
}
