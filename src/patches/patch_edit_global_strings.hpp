#pragma once

#include "game_patch.hpp"
#include "../tools/byte_array.hpp"
#include "../tools/mips_tools.hpp"

class PatchEditGlobalStrings : public GamePatch
{
public:
    void alter_datas_file(BinaryFile& data_file, const GameData& game_data, const RandomizerWorld& world) override
    {
        BinaryFile global_strings_file("./tmp_dump/DATA/ETC_USA.R");

        std::vector<std::string> strings = parse_strings_from_file(global_strings_file);

        // Edit chapter names
        for(size_t i=0 ; i<42 ; ++i)
            strings[i] = "Randomizer";

        // Edit item names
        for(uint8_t item_id = 0 ; item_id < ITEM_COUNT ; ++item_id)
        {
            const std::string& str = game_data.item(item_id)->name();
            if(str.starts_with("Item "))
                continue;

            strings[item_id + 512] = str;
        }

        global_strings_file.clear();
        write_strings_to_file(global_strings_file, strings);
        global_strings_file.save();
    }

private:
    static std::vector<std::string> parse_strings_from_file(const BinaryFile& file)
    {
        std::vector<uint16_t> string_offsets;
        for(uint32_t addr = 0x0 ; addr < 0x800 ; addr += 0x2)
            string_offsets.emplace_back(file.get_word_le(addr));

        std::vector<std::string> strings;
        for(uint16_t offset : string_offsets)
        {
            std::string str;
            if(offset != 0xFFFF)
                str = std::string(reinterpret_cast<const char*>(file.buffer_pointer()) + offset);
            strings.emplace_back(str);
        }

        return strings;
    }

    static void write_strings_to_file(BinaryFile& file, const std::vector<std::string>& strings)
    {
        // Build a dictionary to re-use offsets for identical strings
        std::map<std::string, uint16_t> string_dictionary;

        ByteArray string_bytes;
        ByteArray offset_bytes;

        // Add an empty string at the beginning, and store its offset inside the dictionary
        string_bytes.add_byte(0x00);
        string_dictionary[""] = 0x800;

        for(const std::string& str : strings)
        {
            if(string_dictionary.count(str))
            {
                offset_bytes.add_word_le(string_dictionary[str]);
            }
            else
            {
                offset_bytes.add_word_le(0x800 + string_bytes.size());
                string_dictionary[str] = 0x800 + string_bytes.size();
                string_bytes.add_bytes(reinterpret_cast<const uint8_t*>(str.c_str()), str.size());
                string_bytes.add_byte(0x00);
            }
        }

        file.add_bytes(offset_bytes);
        file.add_bytes(string_bytes);
    }
};
