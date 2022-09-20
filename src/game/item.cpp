#include "item.hpp"
#include "../tools/exception.hpp"

Json Item::to_json() const
{
    Json json;

    json["name"] = _name;
    json["maxQuantity"] = _max_quantity;
    json["startingQuantity"] = _starting_quantity;
    json["goldValue"] = _gold_value;
    if(_is_precious)
        json["precious"] = _is_precious;

    return json;
}

Item* Item::from_json(uint8_t id, const Json& json)
{
    Item* item = new Item();
    item->id(id);
    item->apply_json(json);
    return item;
}

void Item::apply_json(const Json& json)
{
    for(auto& [key, value] : json.items())
    {
        if(key == "name")
            this->name(value);
        else if(key == "maxQuantity")
            this->max_quantity(value);
        else if(key == "startingQuantity")
            this->starting_quantity(value);
        else if(key == "goldValue")
            this->gold_value(value);
        else if(key == "precious")
            this->is_precious(value);
        else
            throw RandomizerException("Unknown key '" + key + "' in Item JSON");
    }
}
