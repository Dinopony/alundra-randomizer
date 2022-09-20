#pragma once

#include <string>
#include <cstdint>

uint8_t get_item_id_from_name(const std::string& name);
const std::string& get_item_name_from_id(uint8_t item_id);
