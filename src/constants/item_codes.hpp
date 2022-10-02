#pragma once

#include <cstdint>

constexpr uint8_t ITEM_NONE	            =  0x00;
constexpr uint8_t ITEM_DAGGER		    =  0x01;
constexpr uint8_t ITEM_SWORD		    =  0x02;
constexpr uint8_t ITEM_FIEND_BLADE		=  0x03;
constexpr uint8_t ITEM_HOLY_SWORD		=  0x04;
constexpr uint8_t ITEM_HUNTERS_BOW		=  0x05;
constexpr uint8_t ITEM_WILLOW_BOW		=  0x06;
constexpr uint8_t ITEM_SPIRIT_WAND		=  0x07;
constexpr uint8_t ITEM_LEGEND_SWORD		=  0x08;
constexpr uint8_t ITEM_IRON_FLAIL		=  0x09;
constexpr uint8_t ITEM_STEEL_FLAIL		=  0x0A;
// 0x0B (used as Sword Upgrade)
// 0x0C (used as Flail Upgrade)
// 0x0D (used as Bow Upgrade)
constexpr uint8_t ITEM_ICE_WAND		    =  0x0E;
// 0x0F (used as Armor Upgrade)
constexpr uint8_t ITEM_FIRE_WAND		=  0x10;
constexpr uint8_t ITEM_CLOTH_ARMOR		=  0x11;
constexpr uint8_t ITEM_LEATHER_ARMOR	=  0x12;
constexpr uint8_t ITEM_ANCIENT_ARMOR    =  0x13;
constexpr uint8_t ITEM_SILVER_ARMOR		=  0x14;
// 0x15 (used as Earth magic upgrade)
// 0x16 (used as Water magic upgrade)
// 0x17 (used as Fire magic upgrade)
// 0x18 (used as Wind magic upgrade)
constexpr uint8_t ITEM_SHORT_BOOTS		=  0x19;
constexpr uint8_t ITEM_LONG_BOOTS		=  0x1A;
constexpr uint8_t ITEM_MERMAN_BOOTS		=  0x1B;
constexpr uint8_t ITEM_CHARM_BOOTS		=  0x1C;
// 0x1D (used as Boots upgrade)
// 0x1E
constexpr uint8_t ITEM_SPRING_BEAN		=  0x1F;
constexpr uint8_t ITEM_SAND_CAPE		=  0x20;
constexpr uint8_t ITEM_BROKEN_ARMOR		=  0x21;
// 0x22
constexpr uint8_t ITEM_BOMB		        =  0x23;
constexpr uint8_t ITEM_HERBS		    =  0x24;
constexpr uint8_t ITEM_STRENGTH_ELIXYR	=  0x25;
constexpr uint8_t ITEM_MAGIC_ELIXYR		=  0x26;
constexpr uint8_t ITEM_WONDER_ESSENCE	=  0x27;
constexpr uint8_t ITEM_AQUA_CAPE		=  0x28;
constexpr uint8_t ITEM_STRENGTH_TONIC	=  0x29;
// 0x2A
constexpr uint8_t ITEM_EARTH_SCROLL		=  0x2B;
constexpr uint8_t ITEM_EARTH_BOOK		=  0x2C;
constexpr uint8_t ITEM_WATER_SCROLL		=  0x2D;
constexpr uint8_t ITEM_WATER_BOOK		=  0x2E;
constexpr uint8_t ITEM_FIRE_SCROLL		=  0x2F;
constexpr uint8_t ITEM_FIRE_BOOK		=  0x30;
constexpr uint8_t ITEM_WIND_SCROLL		=  0x31;
constexpr uint8_t ITEM_WIND_BOOK		=  0x32;
constexpr uint8_t ITEM_OLGAS_RING		=  0x33;
constexpr uint8_t ITEM_OAKS_RING		=  0x34;
constexpr uint8_t ITEM_SILVER_ARMLET	=  0x35;
constexpr uint8_t ITEM_NAVAS_CHARM		=  0x36;
constexpr uint8_t ITEM_RECOVERY_RING	=  0x37;
constexpr uint8_t ITEM_REFRESHER		=  0x38;
constexpr uint8_t ITEM_SECRET_PASS		=  0x39;
// 0x3A
constexpr uint8_t ITEM_POWER_GLOVE		=  0x3B;
constexpr uint8_t ITEM_ELEVATOR_KEY		=  0x3C;
constexpr uint8_t ITEM_KEY		        =  0x3D;
constexpr uint8_t ITEM_RUBY_CREST		=  0x3E;
constexpr uint8_t ITEM_SAPPHIRE_CREST	=  0x3F;
constexpr uint8_t ITEM_TOPAZ_CREST		=  0x40;
constexpr uint8_t ITEM_AGATE_CREST		=  0x41;
constexpr uint8_t ITEM_GARNET_CREST		=  0x42;
constexpr uint8_t ITEM_EMERALD_CREST	=  0x43;
constexpr uint8_t ITEM_DIAMOND_CREST	=  0x44;
constexpr uint8_t ITEM_1_GILDER		    =  0x45;
constexpr uint8_t ITEM_5_GILDER		    =  0x46;
constexpr uint8_t ITEM_10_GILDER		=  0x47;
constexpr uint8_t ITEM_30_GILDER		=  0x48;
constexpr uint8_t ITEM_SLUICE_KEY		=  0x49;
constexpr uint8_t ITEM_BOUQUET		    =  0x4A;
constexpr uint8_t ITEM_SMALL_KEY		=  0x4B;
constexpr uint8_t ITEM_JESS_LETTER		=  0x4C;
constexpr uint8_t ITEM_TREE_GEM		    =  0x4D;
constexpr uint8_t ITEM_ZOLISTS_STONE	=  0x4E;
constexpr uint8_t ITEM_GILDED_FALCON	=  0x4F;
constexpr uint8_t ITEM_MAGIC_SEED		=  0x50;
constexpr uint8_t ITEM_SMALL_CRYSTAL	=  0x51;
constexpr uint8_t ITEM_LARGE_CRYSTAL	=  0x52;
constexpr uint8_t ITEM_LIFE_VESSEL		=  0x53;
constexpr uint8_t ITEM_DEW_OF_LIFE		=  0x54;
constexpr uint8_t ITEM_DROP_OF_LIFE		=  0x55;
constexpr uint8_t ITEM_WATER_OF_LIFE	=  0x56;
constexpr uint8_t ITEM_BOOK_OF_RUNES	=  0x57;
constexpr uint8_t ITEM_BOOK_OF_ELNA		=  0x58;
constexpr uint8_t ITEM_CURIOUS_KEY		=  0x59;

constexpr uint8_t ITEM_COUNT = ITEM_CURIOUS_KEY + 1;

// Items introduced by the randomizer

constexpr uint8_t ITEM_SWORD_UPGRADE        = 0x0B;
constexpr uint8_t ITEM_FLAIL_UPGRADE        = 0x0C;
constexpr uint8_t ITEM_BOW_UPGRADE          = 0x0D;
constexpr uint8_t ITEM_ARMOR_UPGRADE        = 0x0F;

constexpr uint8_t ITEM_EARTH_MAGIC_UPGRADE  = 0x15;
constexpr uint8_t ITEM_WATER_MAGIC_UPGRADE  = 0x16;
constexpr uint8_t ITEM_FIRE_MAGIC_UPGRADE   = 0x17;
constexpr uint8_t ITEM_WIND_MAGIC_UPGRADE   = 0x18;

constexpr uint8_t ITEM_BOOTS_UPGRADE        = 0x1D;
