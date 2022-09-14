#include "bitstream_reader.hpp"

template<>
std::string unpack_from<std::string>(BitstreamReader& bitpack)
{
    std::string str;
    char character;
    while(true) {
        character = (char)bitpack.read_variable_length_number();
        if(character == '\0')
            return str;

        str.push_back(character);
    }
}

template<>
bool unpack_from<bool>(BitstreamReader& bitpack)
{
    return bitpack.next_bit();
}

template<>
uint8_t unpack_from<uint8_t>(BitstreamReader& bitpack)
{
    return (uint8_t)bitpack.read_variable_length_number();
}

template<>
uint16_t unpack_from<uint16_t>(BitstreamReader& bitpack)
{
    return (uint16_t)bitpack.read_variable_length_number();
}

/*
template<>
uint32_t unpack_from<uint32_t>(BitstreamReader& bitpack)
{
    return (uint32_t)bitpack.read_variable_length_number();
}
 */