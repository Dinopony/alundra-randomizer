#include "randomizer_options.hpp"

#include <iostream>
#include <utility>

#include "game/game_data.hpp"
#include "model/randomizer_world.hpp"
#include "model/world_region.hpp"
#include "model/world_node.hpp"
#include "model/item_source.hpp"

#include "tools/vectools.hpp"
#include "tools/bitstream_writer.hpp"
#include "tools/bitstream_reader.hpp"
#include "tools/exception.hpp"
#include "tools/base64.hpp"

RandomizerOptions::RandomizerOptions(const ArgumentDictionary& args, const GameData& game_data, const RandomizerWorld& world)
{
    _items_distribution.fill(0);

    std::string permalink_string = args.get_string("permalink");
    if(!permalink_string.empty())
    {
        // Permalink case: unpack it to find the preset and seed and generate the same world
        this->parse_permalink(permalink_string);
    }
    else
    {
        // Regular case: pick a random seed, read a preset file to get the config and generate a new world
        _seed = (uint32_t) std::chrono::system_clock::now().time_since_epoch().count();

        std::string preset_name = args.get_string("preset");
        stringtools::trim(preset_name);
        if(preset_name.empty())
        {
            std::cout << "Please specify a preset name (name of a file inside the 'presets' folder, leave empty for default): ";
            std::getline(std::cin, preset_name);
            stringtools::trim(preset_name);

            if(preset_name.empty())
                preset_name = "default";
        }

        // If a path was given, filter any kind of directories only keeping the last part of the path
        std::vector<std::string> split_preset_path = stringtools::split(preset_name, "/");
        if(split_preset_path.size() >= 2)
            preset_name = *split_preset_path.rbegin();

        if(!preset_name.ends_with(".json"))
            preset_name += ".json";

        std::string preset_path = "./presets/" + preset_name;
        std::ifstream preset_file(preset_path);
        if(!preset_file)
            throw RandomizerException("Could not open preset file at given path '" + preset_path + "'");

        std::cout << "Preset: '" << preset_path << "'\n";

        Json preset_json;
        preset_file >> preset_json;
        this->apply_json(preset_json, game_data, world);
    }

    this->validate();
}

Json RandomizerOptions::to_json(const GameData& game_data, const RandomizerWorld& world) const
{
    Json json;

    // Game settings
    json["gameSettings"]["originalGameBalance"] = _original_game_balance;
    json["gameSettings"]["megalithsEnabledOnStart"] = _megaliths_enabled_on_start;
    json["gameSettings"]["skipLastDungeon"] = _skip_last_dungeon;

    // Randomizer settings
    json["randomizerSettings"]["allowSpoilerLog"] = _allow_spoiler_log;

    std::map<std::string, uint8_t> items_distribution_with_names;
    for(size_t i=0 ; i < _items_distribution.size() ; ++i)
    {
        uint8_t amount = _items_distribution[i];
        if(amount > 0)
            items_distribution_with_names[game_data.item(i)->name()] = amount;
    }
    json["randomizerSettings"]["itemsDistributions"] = items_distribution_with_names;

    for(auto& [item_source_id, item_id] : _fixed_item_sources)
    {
        ItemSource* item_source = world.item_source(item_source_id);
        WorldRegion* region = item_source->node()->region();
        const Item* item = game_data.item(item_id);
        json["world"]["itemSources"][region->name()][item_source->pretty_name()] = item->name();
    }

    return json;
}

void RandomizerOptions::apply_game_settings_json(const Json& json)
{
    for(auto& [key, value] : json.items())
    {
        if(key == "originalGameBalance")                        
            _original_game_balance = value;
        else if(key == "megalithsEnabledOnStart")
            _megaliths_enabled_on_start = value;
        else if(key == "skipLastDungeon")
            _skip_last_dungeon = value;
        else
            throw RandomizerException("Unknown key '" + key + "' in preset game settings JSON");
    }
}

void RandomizerOptions::apply_randomizer_settings_json(const Json& json, const GameData& game_data)
{
    for(auto& [key, value] : json.items())
    {
        if(key == "allowSpoilerLog")                        
            _allow_spoiler_log = true;
        else if(key == "itemsDistribution")
        {
            std::map<std::string, uint8_t> items_distribution = value;
            for(auto& [item_name, quantity] : items_distribution)
            {
                uint8_t item_id = game_data.item(item_name)->id();
                _items_distribution[item_id] = quantity;
            }
        }
        else
            throw RandomizerException("Unknown key '" + key + "' in preset randomizer settings JSON");
    }
}

void RandomizerOptions::apply_world_json(const Json& json, const GameData& game_data, const RandomizerWorld& world)
{
    if(!json.count("itemSources"))
        return;

    const Json& item_sources_json = json.at("itemSources");
    for(auto& [region_id, item_sources_in_region_json] : item_sources_json.items())
    {
        try {
            WorldRegion* region = world.region(region_id);
            std::vector<ItemSource*> region_item_sources = region->item_sources();
            
            for(auto& [source_name, item_name_json] : item_sources_in_region_json.items())
            {
                ItemSource* source = world.item_source(source_name);
                uint16_t source_id = world.item_source_id(source);
                if(source->node()->region() != region)
                    throw RandomizerException("Item source '" + source_name + "' exists but is not in region '" + region_id +  "'");

                const std::string& item_name = item_name_json;
                const Item* item = game_data.item(item_name);

                _fixed_item_sources[source_id] = item->id();
            }
        } catch(std::out_of_range&) {
            throw RandomizerException("Region '" + region_id + "' could not be found");
        }
    }
}

void RandomizerOptions::apply_json(const Json& json, const GameData& game_data, const RandomizerWorld& world)
{
    if(json.contains("permalink"))
    {
        this->parse_permalink(json.at("permalink"));
        return;
    }

    for(auto& [key, value] : json.items())
    {
        if(key == "permalink")
            continue;
        if(key == "seed")
            _seed = value;
        if(key == "randomizerSettings")                        
            this->apply_randomizer_settings_json(value, game_data);
        else if(key == "gameSettings")
            this->apply_game_settings_json(value);
        else if(key == "world")
            this->apply_world_json(value, game_data, world);
        else
            throw RandomizerException("Unknown key '" + key + "' in preset JSON");
    }
}

void RandomizerOptions::validate() const
{}

std::vector<std::string> RandomizerOptions::hash_words() const
{
    std::vector<std::string> words = {
        "Dagger", "Sword", "Fiend", "Blade", "Holy", "Hunters", "Bow", "Willow", "Spirit", "Wand", "Legend", "Iron",
        "Steel", "Flail", "Ice", "Cloth", "Armor", "Ancient", "Silver", "Short", "Boots", "Long", "Merman",
        "Charm", "Spring", "Bean", "Sand", "Cape", "Aqua", "Broken", "Bomb", "Herbs", "Strength", "Elixyr", "Magic",
        "Wonder", "Essence", "Tonic", "Earth", "Water", "Fire", "Wind", "Scroll", "Book", "Ring", "Armlet",
        "Recovery", "Secret", "Pass", "Power", "Glove", "Elevator", "Key", "Crest", "Ruby", "Sapphire", "Chest"
        "Topaz", "Agate", "Garnet", "Emerald", "Diamond", "Gilder", "Sluice", "Bouquet", "Small", "Letter", "Tree",
        "Gem", "Zolist", "Stone", "Gilded", "Falcon", "Magic", "Seed", "Runes", "Elna", "Curious", "Life", "Vessel",
        "Dreamwalker", "Dream", "Nightmare", "Inoa", "Torla", "Murgg", "Crystal", "Megalith", "Crypt", "Manor", "Desert",
        "Despair", "Cliff", "Madness", "Shrine", "Lake", "Berue", "Casino", "Underground", "Waterway", "Gazeck",
        "Lair", "Tarn", "Coal", "Mine", "Sara", "Coastal", "Cave", "Reptile", "Magyscar", "Sanctuary", "Fairy", "Pond",
        "Golem", "Dragon", "Alundra", "Meia", "Giles", "Lutas", "Merrick", "Jess", "Olga", "Oak", "Nava", "Ronan",
        "Melzas", "Lars", "Bonaire", "Nadia", "Kline", "Wendell", "Olen", "Nirude", "Zazan", "Wilda", "Nestus",
        "Zorgia", "Elene", "Lurvy", "Kohei", "Vul", "Jeal", "Mountain", "Bergus", "Roulette"
    };

    std::mt19937 rng(_seed);
    vectools::shuffle(words, rng);
    return { words.begin(), words.begin()+4 };
}

std::string RandomizerOptions::permalink() const
{
    BitstreamWriter bitpack;

    bitpack.pack((uint8_t)MAJOR_RELEASE);

    bitpack.pack(_seed);
    bitpack.pack(_allow_spoiler_log);
    bitpack.pack_array(_items_distribution);

    bitpack.pack(_original_game_balance);
    bitpack.pack(_megaliths_enabled_on_start);
    bitpack.pack(_skip_last_dungeon);

    bitpack.pack_map(_fixed_item_sources);

    return "a" + base64_encode(bitpack.bytes()) + "/";
}

void RandomizerOptions::parse_permalink(std::string permalink)
{
    stringtools::trim(permalink);
    if(!permalink.starts_with("a") || !permalink.ends_with("/"))
        throw RandomizerException("This permalink is malformed, please make sure you copied the full permalink string.");

    std::vector<uint8_t> bytes = base64_decode(permalink.substr(1, permalink.size() - 2));
    BitstreamReader bitpack(bytes);

    uint8_t version = bitpack.unpack<uint8_t>();
    if(version != (uint8_t)MAJOR_RELEASE)
        throw RandomizerException("This permalink comes from an incompatible version (" + std::to_string(version) + ").");

    _seed = bitpack.unpack<uint32_t>();
    _allow_spoiler_log = bitpack.unpack<bool>();
    _items_distribution = bitpack.unpack_array<uint8_t, ITEM_COUNT>();

    _original_game_balance = bitpack.unpack<bool>();
    _megaliths_enabled_on_start = bitpack.unpack<bool>();
    _skip_last_dungeon = bitpack.unpack<bool>();

    _fixed_item_sources = bitpack.unpack_map<uint16_t, uint8_t>();
}
