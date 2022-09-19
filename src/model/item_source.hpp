#pragma once

#include <string>
#include <utility>
#include "item.hpp"
#include "../constants/item_codes.hpp"

class World;

class ItemSource
{
private:
    /// The name used to describe this ItemSource
    std::string _name;
    /// The item contained inside this ItemSource
    Item* _item = nullptr;
    /// The item contained inside this ItemSource in the base game
    Item* _vanilla_item = nullptr;
    /// The ID of the logic node containing this ItemSource
    std::string _node_id;
    /// Addresses inside DATAS.BIN file where to put the contained item ID
    std::vector<uint32_t> _addresses;
    /// Addresses inside DATAS.BIN file where to put the contained item sprite ID
    std::vector<uint32_t> _sprite_addresses;
    /// Address inside ALUND_CD.EXE file where to put the contained item ItemInfo address (0 if not handled)
    uint32_t _merrick_item_address = 0x0;
    /// A boolean indicating whether this item source can hold an item required for progression or not
    bool _can_contain_progression = true;
    /// An array of hints which can be used to subtly point at this ItemSource
    std::vector<std::string> _hints;

public:
    ItemSource() = default;
    virtual ~ItemSource() = default;

    [[nodiscard]] const std::string& name() const { return _name; }
    void name(const std::string& name) { _name = name; }
    [[nodiscard]] std::string pretty_name() const;

    [[nodiscard]] Item* item() const { return _item; }
    virtual void item(Item* item) { _item = item; }
    /// A source is empty if it contains no item, but not if it contains ITEM_NONE
    [[nodiscard]] bool empty() const { return _item == nullptr; }

    [[nodiscard]] Item* vanilla_item() const { return _vanilla_item; }

    [[nodiscard]] const std::string& node_id() const { return _node_id; }
    void node_id(const std::string& node_id) { _node_id = node_id; }

    [[nodiscard]] const std::vector<std::string>& hints() const { return _hints; }
    void add_hint(const std::string& hint) { _hints.emplace_back(hint); }

    bool can_contain_progression() const { return _can_contain_progression; }
    void can_contain_progression(bool value) { _can_contain_progression = value; }

    [[nodiscard]] const std::vector<uint32_t>& addresses() const { return _addresses; }
    void add_address(uint32_t address) { _addresses.emplace_back(address); }

    [[nodiscard]] const std::vector<uint32_t>& sprite_addresses() const { return _sprite_addresses; }
    void add_sprite_address(uint32_t address) { _sprite_addresses.emplace_back(address); }

    [[nodiscard]] uint32_t merrick_item_address() const { return _merrick_item_address; }
    void merrick_item_address(uint32_t addr) { _merrick_item_address = addr; }

    [[nodiscard]] virtual Json to_json() const;
    static ItemSource* from_json(const Json& json, const World& world);
};
