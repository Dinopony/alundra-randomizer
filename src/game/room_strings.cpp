#include "room_strings.hpp"
#include "../tools/binary_file.hpp"
#include "../tools/exception.hpp"

RoomStrings::RoomStrings(uint32_t map_id, const BinaryFile& datas_file)
{
    uint32_t room_offset_addr = 0x28 + (map_id * 4);
    uint32_t room_addr = datas_file.get_long_le(room_offset_addr);
    uint32_t next_room_addr = datas_file.get_long_le(room_offset_addr + 4);
    if(!next_room_addr)
        next_room_addr = datas_file.size();
    uint32_t room_bytes_size = next_room_addr - room_addr;

    uint32_t sector_7_offset = datas_file.get_long_le(room_addr + 0x18);
    _initial_strings_size = room_bytes_size - sector_7_offset;
    _initial_strings_pos = room_addr + sector_7_offset;
    this->parse_strings(datas_file, _initial_strings_pos);
}

void RoomStrings::parse_strings(const BinaryFile& datas_file, uint32_t starting_offset)
{
    for(uint32_t addr = starting_offset ;  ; addr += 2)
    {
        uint32_t string_offset = datas_file.get_word_le(addr);
        if(string_offset == 0xFFFF)
            break;

        string_offset += starting_offset;
        const char* string_pointer = reinterpret_cast<const char*>(datas_file.buffer_pointer()) + string_offset;
        _strings.emplace_back(std::string(string_pointer));
    }
}

Json RoomStrings::to_json() const
{
    Json strings_json = Json::array();
    for(auto& str : _strings)
        strings_json.emplace_back(str);
    return strings_json;
}

void RoomStrings::apply_on_data(BinaryFile& datas_file) const
{
    ByteArray string_bytes;
    uint16_t offsets_table_size = (_strings.size() + 1) * 2;
    string_bytes.reserve(offsets_table_size + _strings.size() * 20);
    string_bytes.resize(offsets_table_size, 0xFF);

    for(size_t i=0 ; i<_strings.size() ; ++i)
    {
        std::string str = _strings[i];
        char* str_ptr = &(str[0]);
        string_bytes.set_word_le(i * 2, string_bytes.size());
        string_bytes.add_bytes(reinterpret_cast<uint8_t*>(str_ptr), str.size() + 1);
    }

    if(string_bytes.size() > _initial_strings_size)
        throw RandomizerException("String data is bigger than in original room");

    string_bytes.resize(_initial_strings_size, 0x00);
    datas_file.set_bytes(_initial_strings_pos, string_bytes);
}

