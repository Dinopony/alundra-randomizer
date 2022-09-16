#include "randomizer_options.hpp"

#include <iostream>
#include <utility>

#include "tools/vectools.hpp"
#include "tools/bitstream_writer.hpp"
#include "tools/bitstream_reader.hpp"
#include "tools/exception.hpp"

#include "tools/base64.hpp"

RandomizerOptions::RandomizerOptions(const ArgumentDictionary& args, std::array<std::string, ITEM_COUNT>  item_names) :
    _item_names(std::move(item_names))
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
        this->parse_json(preset_json);
    }

    this->validate();
}

Json RandomizerOptions::to_json() const
{
    Json json;

    // Game settings
    json["gameSettings"]["originalGameBalance"] = _original_game_balance;
    json["gameSettings"]["megalithsEnabledOnStart"] = _megaliths_enabled_on_start;

    // Randomizer settings
    json["randomizerSettings"]["allowSpoilerLog"] = _allow_spoiler_log;

    std::map<std::string, uint8_t> items_distribution_with_names;
    for(size_t i=0 ; i < _items_distribution.size() ; ++i)
    {
        uint8_t amount = _items_distribution[i];
        const std::string& item_name = _item_names[i];
        if(amount > 0)
            items_distribution_with_names[item_name] = amount;
    }
    json["randomizerSettings"]["itemsDistributions"] = items_distribution_with_names;

    return json;
}

void RandomizerOptions::parse_json(const Json& json)
{
    if(json.contains("permalink"))
    {
        this->parse_permalink(json.at("permalink"));
        return;
    }

    if(json.contains("gameSettings"))
    {
        const Json& game_settings_json = json.at("gameSettings");

        if(game_settings_json.contains("originalGameBalance"))
            _original_game_balance = game_settings_json.at("originalGameBalance");
        if(game_settings_json.contains("megalithsEnabledOnStart"))
            _megaliths_enabled_on_start = game_settings_json.at("megalithsEnabledOnStart");
    }

    if(json.contains("randomizerSettings"))
    {
        const Json& randomizer_settings_json = json.at("randomizerSettings");

        if(randomizer_settings_json.contains("allowSpoilerLog"))
            _allow_spoiler_log = randomizer_settings_json.at("allowSpoilerLog");

      if(randomizer_settings_json.contains("itemsDistribution"))
      {
          std::map<std::string, uint8_t> items_distribution = randomizer_settings_json.at("itemsDistribution");
          for(auto& [item_name, quantity] : items_distribution)
          {
              auto it = std::find(_item_names.begin(), _item_names.end(), item_name);
              if(it == _item_names.end())
                  throw RandomizerException("Unknown item name '" + item_name + "' in items distribution section of preset file.");
              uint8_t item_id = std::distance(_item_names.begin(), it);
              _items_distribution[item_id] = quantity;
          }
      }
    }

    if(json.contains("seed"))
        _seed = json.at("seed");
}

void RandomizerOptions::validate() const
{}

std::vector<std::string> RandomizerOptions::hash_words() const
{
    std::vector<std::string> words = {
        "Dagger", "Sword", "Fiend", "Blade", "Holy", "Hunters", "Bow", "Willow", "Spirit", "Wand", "Legend", "Iron",
        "Steel", "Flail", "Ice", "Fire", "Cloth", "Armor", "Ancient", "Silver", "Short", "Boots", "Long", "Merman",
        "Charm", "Spring", "Bean", "Sand", "Cape", "Aqua", "Broken", "Bomb", "Herbs", "Strength", "Elixyr", "Magic",
        "Wonder", "Essence", "Tonic", "Earth", "Water", "Fire", "Wind", "Scroll", "Book",  "Ring", "Armlet",
        "Recovery", "Ring", "Secret", "Pass", "Power", "Glove", "Elevator", "Key", "Crest", "Ruby", "Sapphire",
        "Topaz", "Agate", "Garnet", "Emerald", "Diamond", "Gilder", "Sluice", "Bouquet", "Small", "Letter", "Tree",
        "Gem", "Zolist", "Stone", "Gilded", "Falcon", "Magic", "Seed", "Runes", "Elna", "Curious", "Life", "Vessel",
        "Dreamwalker", "Dream", "Nightmare", "Inoa", "Torla", "Murgg", "Crystal", "Megalith", "Crypt", "Manor", "Desert",
        "Despair", "Cliff", "Madness", "Shrine", "Lake", "Ancient", "Desert", "Berue", "Casino", "Underground", "Waterway",
        "Lair", "Tarn", "Coal", "Mine", "Sara", "Coastal", "Cave", "Reptile", "Magyscar", "Sanctuary", "Fairy", "Pond",
        "Golem", "Dragon", "Alundra", "Meia", "Giles", "Lutas", "Merrick", "Jess", "Olga", "Oak", "Nava", "Ronan",
        "Melzas", "Lars", "Bonaire", "Nadia", "Kline", "Wendell", "Olen", "Nirude", "Zazan", "Wilda", "Nestus",
        "Zorgia", "Elene", "Lurvy", "Kohei"
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
}
