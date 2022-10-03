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

    /// The value of this item in shops
    uint16_t _gold_value = 0;

    /// The ID of the slot where this item goes in inventory
    uint16_t _slot_id = 0;

    /// The tier of this item compared to other items from the same slot
    uint16_t _tier = 0;

    /**
     * If true, this item is considered as precious and cannot be put inside ItemSources which forbid precious
     * items. This is used on ItemSources than can be taken an infinite amount of times to avoid putting
     * abusable items such as Life Vessels or Magic Seeds.
     */
    bool _is_precious = false;

public:
    Item() = default;

    [[nodiscard]] uint8_t id() const { return _id; }
    void id(uint8_t id) { _id = id; }

    [[nodiscard]] const std::string& name() const { return _name; }
    void name(const std::string& name) { _name = name; }

    [[nodiscard]] uint8_t max_quantity() const { return _max_quantity; }
    void max_quantity(uint8_t quantity) { _max_quantity = quantity; }

    [[nodiscard]] uint16_t gold_value() const { return _gold_value; }
    void gold_value(uint16_t value) { _gold_value = value; }

    [[nodiscard]] uint16_t slot_id() const { return _slot_id; }
    void slot_id(uint16_t value) { _slot_id = value; }

    [[nodiscard]] uint16_t tier() const { return _tier; }
    void tier(uint16_t value) { _tier = value; }

    [[nodiscard]] bool is_precious() const { return _is_precious; }
    void is_precious(bool value) { _is_precious = value; }

    [[nodiscard]] Json to_json() const;
    static Item* from_json(uint8_t id, const Json& json);
    void apply_json(const Json& json);
};
