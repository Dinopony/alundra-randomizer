#pragma once

#include <string>
#include <utility>
#include "../game/item.hpp"
#include "../constants/item_codes.hpp"

class GameData;
class RandomizerWorld;
class WorldNode;

class ItemSource
{
private:
    /// The name used to describe this ItemSource
    std::string _name;

    /// The item contained inside this ItemSource
    const Item* _item = nullptr;

    /// The item contained inside this ItemSource in the base game
    const Item* _vanilla_item = nullptr;

    /// The logic node containing this ItemSource
    WorldNode* _node = nullptr;

    /// Addresses inside DATAS.BIN file where to put the contained item ID
    std::vector<uint32_t> _addresses;

    /// Addresses inside DATAS.BIN file where to put the contained item sprite ID
    std::vector<uint32_t> _sprite_addresses;

    /// Address inside ALUND_CD.EXE file where to put the contained item ItemInfo address (0 if not handled)
    uint32_t _merrick_item_address = 0x0;

    /// A boolean indicating whether this item source can hold an item required for progression or not
    bool _can_contain_progression = true;

    /// If set to true, this ItemSource cannot contain "precious items" (items that would break the game if taken
    /// repeatedly). This is usually set for ItemSources that can be taken an infinite amount of times.
    bool _forbid_precious_items = false;

    /// An array of hints which can be used to subtly point at this ItemSource
    std::vector<std::string> _hints;

public:
    ItemSource() = default;
    virtual ~ItemSource() = default;

    [[nodiscard]] const std::string& name() const { return _name; }
    void name(const std::string& name) { _name = name; }
    [[nodiscard]] std::string pretty_name() const;

    [[nodiscard]] const Item* item() const { return _item; }
    virtual void item(const Item* item) { _item = item; }

    /// A source is empty if it contains no item, but not if it contains ITEM_NONE
    [[nodiscard]] bool is_empty() const { return _item == nullptr; }

    [[nodiscard]] const Item* vanilla_item() const { return _vanilla_item; }
    void vanilla_item(const Item* item) { _vanilla_item = item; }

    [[nodiscard]] WorldNode* node() const { return _node; }
    void node(WorldNode* node);

    [[nodiscard]] const std::vector<std::string>& hints() const { return _hints; }
    void add_hint(const std::string& hint) { _hints.emplace_back(hint); }

    [[nodiscard]] bool can_contain_progression() const { return _can_contain_progression; }
    void can_contain_progression(bool value) { _can_contain_progression = value; }

    [[nodiscard]] const std::vector<uint32_t>& addresses() const { return _addresses; }
    void addresses(const std::vector<uint32_t>& addresses) { _addresses = addresses; }
    void add_address(uint32_t address) { _addresses.emplace_back(address); }

    [[nodiscard]] const std::vector<uint32_t>& sprite_addresses() const { return _sprite_addresses; }
    void sprite_addresses(const std::vector<uint32_t>& addresses) { _sprite_addresses = addresses; }
    void add_sprite_address(uint32_t address) { _sprite_addresses.emplace_back(address); }

    [[nodiscard]] uint32_t merrick_item_address() const { return _merrick_item_address; }
    void merrick_item_address(uint32_t addr) { _merrick_item_address = addr; }

    [[nodiscard]] bool forbid_precious_items() const { return _forbid_precious_items; }
    void forbid_precious_items(bool value) { _forbid_precious_items = value; }

    [[nodiscard]] virtual Json to_json() const;
    static ItemSource* from_json(const Json& json, const GameData& game_data, const RandomizerWorld& world);
};
