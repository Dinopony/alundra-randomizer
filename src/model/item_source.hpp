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

    std::vector<uint32_t> _addresses; ///< The address of the item source contents inside the DATAS.BIN file

public:
    ItemSource(std::string name, std::vector<uint32_t> addresses, const std::string& node_id, const std::vector<std::string>& hints);
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

    [[nodiscard]] virtual Json to_json() const;
    static ItemSource* from_json(const Json& json, const World& world);
};
