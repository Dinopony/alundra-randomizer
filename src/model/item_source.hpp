#pragma once

#include <string>
#include <utility>
#include "item.hpp"
#include "../constants/item_codes.hpp"

class World;

class ItemSource
{
private:
    std::string _name;
    Item* _item = nullptr;
    std::string _node_id;
    std::vector<std::string> _hints;

    /// Addresses inside DATAS.BIN file where to put the contained item ID
    std::vector<uint32_t> _addresses;
    /// Addresses inside DATAS.BIN file where to put the contained item sprite ID
    std::vector<uint32_t> _sprite_addresses;
    /// Address inside ALUND_CD.EXE file where to put the contained item ItemInfo address (0 if not handled)
    uint32_t _merrick_item_address = 0x0;

public:
    ItemSource() = default;
    virtual ~ItemSource() = default;

    [[nodiscard]] const std::string& name() const { return _name; }
    void name(const std::string& name) { _name = name; }

    [[nodiscard]] Item* item() const { return _item; }
    virtual void item(Item* item) { _item = item; }

    /// A source is empty if it contains no item, but not if it contains ITEM_NONE
    [[nodiscard]] bool empty() const { return _item == nullptr; }

    [[nodiscard]] const std::string& node_id() const { return _node_id; }
    void node_id(const std::string& node_id) { _node_id = node_id; }

    [[nodiscard]] const std::vector<std::string>& hints() const { return _hints; }
    void add_hint(const std::string& hint) { _hints.emplace_back(hint); }

    [[nodiscard]] const std::vector<uint32_t>& addresses() const { return _addresses; }
    void add_address(uint32_t address) { _addresses.emplace_back(address); }

    [[nodiscard]] const std::vector<uint32_t>& sprite_addresses() const { return _sprite_addresses; }
    void add_sprite_address(uint32_t address) { _sprite_addresses.emplace_back(address); }

    [[nodiscard]] uint32_t merrick_item_address() const { return _merrick_item_address; }
    void merrick_item_address(uint32_t addr) { _merrick_item_address = addr; }

    [[nodiscard]] virtual Json to_json() const;
    static ItemSource* from_json(const Json& json, const World& world);
};
