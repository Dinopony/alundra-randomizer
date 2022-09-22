#pragma once

#include "../tools/json.hpp"
#include <sstream>
#include <utility>

class Item
{
private:
    /// The in-game ID of the item
    uint8_t _id;
    
    /// The name of the item
    std::string _name;

    /// The maximum amount of this item the player can carry at any given point
    uint8_t _max_quantity = 1;

    /// The amount possessed by player on game start
    uint8_t _starting_quantity = 0;

    /// The value of this item in shops
    uint16_t _gold_value = 0;

    /**
     * If true, this item is considered as precious and cannot be put inside ItemSources which forbid precious
     * items. This is used on ItemSources than can be taken an infinite amount of times to avoid putting
     * abusable items such as Life Vessels or Magic Seeds.
     */
    bool _is_precious = false;

public:
    Item() = default;

    [[nodiscard]] uint8_t id() const { return _id; }
    Item& id(uint8_t id) { _id = id; return *this; }

    [[nodiscard]] const std::string& name() const { return _name; }
    Item& name(const std::string& name) { _name = name; return *this; }

    [[nodiscard]] uint8_t starting_quantity() const { return std::min(_starting_quantity, _max_quantity); }
    Item& starting_quantity(uint8_t quantity) { _starting_quantity = quantity; return *this; }
    
    [[nodiscard]] uint8_t max_quantity() const { return _max_quantity; }
    Item& max_quantity(uint8_t quantity) { _max_quantity = quantity; return *this; }

    [[nodiscard]] uint16_t gold_value() const { return _gold_value; }
    virtual Item& gold_value(uint16_t value) { _gold_value = value; return *this; }

    [[nodiscard]] bool is_precious() const { return _is_precious; }
    void is_precious(bool value) { _is_precious = value; }

    [[nodiscard]] Json to_json() const;
    static Item* from_json(uint8_t id, const Json& json);
    void apply_json(const Json& json);
};
