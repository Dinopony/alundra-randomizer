#include "item_names.hpp"
#include "item_codes.hpp"
#include "../tools/exception.hpp"
#include <stdexcept>
#include <map>

static std::map<std::string, uint8_t> ITEM_IDS = {
    { "Nothing",            ITEM_NONE },
    { "Dagger",             ITEM_DAGGER },
    { "Sword",              ITEM_SWORD },
    { "Fiend Blade",        ITEM_FIEND_BLADE },
    { "Holy Sword",         ITEM_HOLY_SWORD },
    { "Hunter's Bow",       ITEM_HUNTERS_BOW },
    { "Willow Bow",         ITEM_WILLOW_BOW },
    { "Spirit Wand",        ITEM_SPIRIT_WAND },
    { "Legend Sword",       ITEM_LEGEND_SWORD },
    { "Iron Flail",         ITEM_IRON_FLAIL },
    { "Steel Flail",        ITEM_STEEL_FLAIL },
    { "Ice Wand",           ITEM_ICE_WAND },
    { "Fire Wand",          ITEM_FIRE_WAND },
    { "Cloth Armor",        ITEM_CLOTH_ARMOR },
    { "Leather Armor",      ITEM_LEATHER_ARMOR },
    { "Ancient Armor",      ITEM_ANCIENT_ARMOR },
    { "Silver Armor",       ITEM_SILVER_ARMOR },
    { "Short Boots",        ITEM_SHORT_BOOTS },
    { "Long Boots",         ITEM_LONG_BOOTS },
    { "Merman Boots",       ITEM_MERMAN_BOOTS },
    { "Charm Boots",        ITEM_CHARM_BOOTS },
    { "Spring Bean",        ITEM_SPRING_BEAN },
    { "Sand Cape",          ITEM_SAND_CAPE },
    { "Broken Armor",       ITEM_BROKEN_ARMOR },
    { "Bomb",               ITEM_BOMB },
    { "Herbs",              ITEM_HERBS },
    { "Strength Elixyr",    ITEM_STRENGTH_ELIXYR },
    { "Magic Elixyr",       ITEM_MAGIC_ELIXYR },
    { "Wonder Essence",     ITEM_WONDER_ESSENCE },
    { "Aqua Cape",          ITEM_AQUA_CAPE },
    { "Strength Tonic",     ITEM_STRENGTH_TONIC },
    { "Earth Scroll",       ITEM_EARTH_SCROLL },
    { "Earth Book",         ITEM_EARTH_BOOK },
    { "Water Scroll",       ITEM_WATER_SCROLL },
    { "Water Book",         ITEM_WATER_BOOK },
    { "Fire Scroll",        ITEM_FIRE_SCROLL },
    { "Fire Book",          ITEM_FIRE_BOOK },
    { "Wind Scroll",        ITEM_WIND_SCROLL },
    { "Wind Book",          ITEM_WIND_BOOK },
    { "Olgas Ring",         ITEM_OLGAS_RING },
    { "Silver Armlet",      ITEM_SILVER_ARMLET },
    { "Nava's Charm",       ITEM_NAVAS_CHARM },
    { "Recovery Ring",      ITEM_RECOVERY_RING },
    { "Secret Pass",        ITEM_SECRET_PASS },
    { "Power Glove",        ITEM_POWER_GLOVE },
    { "Elevator Key",       ITEM_ELEVATOR_KEY },
    { "Key",                ITEM_KEY },
    { "Ruby Crest",         ITEM_RUBY_CREST },
    { "Sapphire Crest",     ITEM_SAPPHIRE_CREST },
    { "Topaz Crest",        ITEM_TOPAZ_CREST },
    { "Agate Crest",        ITEM_AGATE_CREST },
    { "Garnet Crest",       ITEM_GARNET_CREST },
    { "Emerald Crest",      ITEM_EMERALD_CREST },
    { "Diamond Crest",      ITEM_DIAMOND_CREST },
    { "1 Gilder",           ITEM_1_GILDER },
    { "5 Gilder",           ITEM_5_GILDER },
    { "10 Gilder",          ITEM_10_GILDER },
    { "30 Gilder",          ITEM_30_GILDER },
    { "Sluice Key",         ITEM_SLUICE_KEY },
    { "Bouquet",            ITEM_BOUQUET },
    { "Small Key",          ITEM_SMALL_KEY },
    { "Jess's Letter",      ITEM_JESS_LETTER },
    { "Tree Gem",           ITEM_TREE_GEM },
    { "Zolists Stone",      ITEM_ZOLISTS_STONE },
    { "Gilded Falcon",      ITEM_GILDED_FALCON },
    { "Magic Seed",         ITEM_MAGIC_SEED },
    { "Life Vessel",        ITEM_LIFE_VESSEL },
    { "Book of Runes",      ITEM_BOOK_OF_RUNES },
    { "Book of Elna",       ITEM_BOOK_OF_ELNA },
    { "Curious Key",        ITEM_CURIOUS_KEY }
};

uint8_t get_item_id_from_name(const std::string& name)
{
    try {
        return ITEM_IDS.at(name);
    } catch(std::out_of_range&) {
        throw RandomizerException("Couldn't find item with name '" + name + "'.");
    }
}

const std::string& get_item_name_from_id(uint8_t item_id)
{
    for(auto& [name, id] : ITEM_IDS)
    {
        if(id == item_id)
            return name;
    }

    throw RandomizerException("Couldn't find item with ID " + std::to_string(item_id) + ".");
}