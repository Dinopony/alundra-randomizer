#pragma once

#include <string>
#include "../tools/json.hpp"
#include "../tools/byte_array.hpp"

class BinaryFile;

class RoomStrings {
private:
    uint32_t _initial_strings_pos;
    uint32_t _initial_strings_size;
    std::vector<std::string> _strings;

public:
    RoomStrings(uint32_t map_id, const BinaryFile& datas_file);

    void set_string(size_t string_id, const std::string& str);

    void apply_on_data(BinaryFile& datas_file) const;
    [[nodiscard]] Json to_json() const;

private:
    void parse_strings(const BinaryFile& datas_file, uint32_t starting_offset);

};
