#include "io.hpp"

#include "../model/world_node.hpp"
#include "../model/world_region.hpp"
#include "../model/randomizer_world.hpp"
#include "../game/game_data.hpp"
#include "../tools/stringtools.hpp"
#include "../tools/exception.hpp"

static void parse_item_sources_from_json(const Json& json, RandomizerWorld& world, GameData& game_data)
{
    if(!json.count("itemSources"))
        return;

    const Json& item_sources_json = json.at("itemSources");
    for(auto&[region_id, item_sources_in_region_json] : item_sources_json.items())
    {
        try {
            WorldRegion* region = world.region(region_id);
            std::map<std::string, ItemSource*> region_item_sources = region->item_sources();

            for(auto&[source_name, item_name_json] : item_sources_in_region_json.items())
            {
                try {
                    ItemSource* source = region_item_sources.at(source_name);
                    const std::string& item_name = item_name_json;
                    const Item* item = game_data.item(item_name);
                    source->item(item);
                } catch(std::out_of_range&) {
                    throw RandomizerException("Item source '" + source_name + "' could not be found in region '" + region_id +  "'");
                }
            }
        } catch(std::out_of_range&) {
            throw RandomizerException("Region '" + region_id + "' could not be found");
        }
    }
}

void WorldJsonParser::parse_world_json(const Json& json, RandomizerWorld& world, GameData& game_data)
{
    parse_item_sources_from_json(json, world, game_data);
}
