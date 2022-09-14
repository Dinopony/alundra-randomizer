#pragma once

#include "../tools/json.hpp"
#include <sstream>
#include <utility>

class Item
{
private:
    uint8_t     _id = 0xFF;
    std::string _name;
    uint8_t     _max_quantity = 0;
    uint8_t     _starting_quantity = 0;
    uint16_t    _gold_value = 0;

public:
    Item() = default;
    Item(uint8_t id, std::string name, uint8_t max_quantity, uint8_t starting_quantity, uint16_t gold_value) :
        _id                 (id),
        _name               (std::move(name)),
        _max_quantity       (max_quantity),
        _starting_quantity  (starting_quantity),
        _gold_value         (gold_value)
    {}
    
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

    [[nodiscard]] Json to_json() const;
    static Item* from_json(uint8_t id, const Json& json);
    void apply_json(const Json& json);
};
