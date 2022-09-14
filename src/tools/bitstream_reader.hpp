#pragma once

#include <vector>
#include <map>
#include "exception.hpp"

class BitstreamReader {
private:
    const uint8_t* _data;
    int8_t _bit_position = 7;
    size_t _size = 0;

public:
    explicit BitstreamReader(const std::vector<uint8_t>& data) : _data(&(data[0]))
    {}

    explicit BitstreamReader(const uint8_t* data) : _data(data)
    {}

    [[nodiscard]] size_t size() const { return _size; }

    bool next_bit()
    {
        if(_bit_position < 0)
        {
            // First byte was fully read already, reset bit position and go onto next byte
            _bit_position = 7;
            _data++;
            _size++;
        }

        bool first_bit = (*_data >> _bit_position) & 0x01;
        _bit_position -= 1;

        return first_bit;
    }

    uint32_t read_bits(uint8_t amount)
    {
        if(amount > 32)
            throw RandomizerException("Cannot read more than 32 bits using Bitstream");

        uint32_t value = 0;
        for(uint8_t i=0 ; i<amount ; ++i)
        {
            value <<= 1;
            value |= this->next_bit() ? 1 : 0;
        }

        return value;
    }

    void skip_byte_remainder()
    {
        if(_bit_position != 7)
        {
            _bit_position = 7;
            _data++;
            _size++;
        }
    }

    /**
     * Gets compressed variable-width number. Number is in the form 2^Exp + Man
     * Exp is the number of leading zeroes. The following bits make up the
     * mantissa. The same number of bits make up the exponent and mantissa
     */
    uint32_t read_variable_length_number()
    {
        uint16_t exponent = 0;
        while(!this->next_bit())
            exponent += 1;

        uint32_t mantissa = this->read_bits(exponent);
        return (1 << exponent) + mantissa - 1;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////

    template<typename T>
    T unpack()
    {
        return unpack_from<T>(*this);
    }

    template<typename T>
    std::vector<T> unpack_vector()
    {
        std::vector<T> ret;

        uint16_t vector_size = this->unpack<uint16_t>();
        for(uint16_t i=0 ; i<vector_size ; ++i)
            ret.emplace_back(this->unpack<T>());

        return ret;
    }

    template<typename T, size_t N>
    std::array<T,N> unpack_array()
    {
        std::array<T,N> ret;
        for(size_t i=0 ; i<N ; ++i)
            ret[i] = this->unpack<T>();

        return ret;
    }

    template<typename K, typename V>
    std::map<K,V> unpack_map()
    {
        std::map<K,V> ret;

        uint16_t map_size = this->unpack<uint16_t>();
        for (uint16_t i = 0; i < map_size; ++i)
        {
            K key = this->unpack<K>();
            V value = this->unpack<V>();
            ret[key] = value;
        }

        return ret;
    }
};

template<typename T>
T unpack_from(BitstreamReader& bitpack)
{
    T value = 0;
    uint8_t* pointer_on_value = reinterpret_cast<uint8_t*>(&value);
    for(size_t i=0 ; i<sizeof(T) ; ++i)
    {
        for(uint8_t j=0 ; j<8 ; ++j)
        {
            uint8_t bit = bitpack.next_bit() ? 1 : 0;
            bit <<= (7-j);
            *pointer_on_value |= bit;
        }
        pointer_on_value += 1;
    }
    return value;
}

template<>
std::string unpack_from<std::string>(BitstreamReader& bitpack);

template<>
bool unpack_from<bool>(BitstreamReader& bitpack);

template<>
uint8_t unpack_from<uint8_t>(BitstreamReader& bitpack);

template<>
uint16_t unpack_from<uint16_t>(BitstreamReader& bitpack);

/*
template<>
uint32_t unpack_from<uint32_t>(BitstreamReader& bitpack);
*/