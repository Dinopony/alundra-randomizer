#pragma once

#include <vector>
#include "exception.hpp"

class BitstreamWriter {
private:
    std::vector<uint8_t> _bytes;
    int8_t _bit_position = 8;

public:
    BitstreamWriter() = default;

    [[nodiscard]] const std::vector<uint8_t>& bytes() const { return _bytes; }

    void add_bit(bool bit)
    {
        if(_bit_position == 8)
        {
            _bytes.emplace_back(0);
            _bit_position = 0;
        }

        uint8_t& last_byte = _bytes[_bytes.size()-1];
        uint8_t base_value = (bit) ? 1 : 0;
        last_byte |= (base_value << (7 - _bit_position));
        _bit_position += 1;
    }

    void add_bits(const std::vector<bool>& bits)
    {
        for(bool bit : bits)
            this->add_bit(bit);
    }

    void add_number(uint32_t value, int num_bits)
    {
        for(int bit = num_bits-1 ; bit >= 0 ; --bit)
            this->add_bit(value & (1 << bit));
    }

    void skip_byte_remainder()
    {
        if(_bit_position != 0)
            _bit_position = 8;
    }

    void add_variable_length_number(uint32_t number)
    {
        uint16_t exponent = 0;
        uint32_t number_copy = number + 1;
        while(number_copy > 1) {
            exponent += 1;
            number_copy >>= 1;
        }
        uint32_t mantissa = number + 1 - (1 << exponent);

        for(uint16_t i=0 ; i<exponent ; ++i)
            this->add_bit(false);
        this->add_bit(true);
        this->add_number(mantissa, exponent);
    }

    ////////////////////////////////////////////////////////////

    template<typename T>
    void pack_pod_type(T& value)
    {
        uint8_t* value_as_bytes = reinterpret_cast<uint8_t*>(&value);
        std::vector<uint8_t> value_as_bytes_vec(value_as_bytes, value_as_bytes + sizeof(T));

        std::vector<bool> value_as_bits;
        value_as_bits.reserve(value_as_bytes_vec.size() * 8);
        for(uint8_t byte : value_as_bytes_vec)
            for(uint8_t i=0 ; i<8 ; ++i)
                this->add_bit(byte & (1 << (7 - i)));
    }

    template<typename T>
    void pack_vector(const std::vector<T>& vector)
    {
        this->pack((uint16_t)vector.size());
        for(const T& elem : vector)
            this->pack(elem);
    }

    template<typename T, size_t N>
    void pack_array(const std::array<T,N>& array)
    {
        for(const T& elem : array)
            this->pack(elem);
    }

    template<typename K, typename V>
    void pack_map(const std::map<K,V>& map)
    {
        this->pack((uint16_t)map.size());
        for(auto& [k,v] : map)
        {
            this->pack((K)k);
            this->pack((V)v);
        }
    }

    void pack(uint8_t value)       { this->add_variable_length_number(value); }
    void pack(uint16_t value)      { this->add_variable_length_number(value); }
    void pack(uint32_t value)      { this->pack_pod_type(value); }
    void pack(float value)         { this->pack_pod_type(value); }
    void pack(double value)        { this->pack_pod_type(value); }
    void pack(bool value)          { this->add_bit(value); }

    void pack(const std::string& value)
    {
        for(uint8_t character : value)
            this->pack(character);
        this->pack((uint8_t) '\0');
    }
};
