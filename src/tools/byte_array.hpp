#pragma once

#include <vector>
#include <cstdint>

class ByteArray : public std::vector<uint8_t>
{
public:
    ByteArray() = default;

    explicit ByteArray(const std::vector<uint8_t>& vec) {
        this->insert(this->end(), vec.begin(), vec.end());
    }

    [[nodiscard]] uint8_t get_byte(uint32_t address) const;
    [[nodiscard]] uint16_t get_word(uint32_t address) const;
    [[nodiscard]] uint32_t get_long(uint32_t address) const;
    [[nodiscard]] uint16_t get_word_le(uint32_t address) const;
    [[nodiscard]] uint32_t get_long_le(uint32_t address) const;
    [[nodiscard]] ByteArray get_bytes(uint32_t begin, uint32_t end) const;

    void add_byte(uint8_t byte);
    void add_word(uint16_t word);
    void add_long(uint32_t long_word);
    void add_word_le(uint16_t word);
    void add_long_le(uint32_t long_word);
    void add_bytes(const std::vector<uint8_t>& bytes);
    void add_bytes(const uint8_t* bytes, size_t bytes_size);

    void set_byte(uint32_t address, uint8_t byte);
    void set_word(uint32_t address, uint16_t word);
    void set_long(uint32_t address, uint32_t long_word);
    void set_word_le(uint32_t address, uint16_t word);
    void set_long_le(uint32_t address, uint32_t long_word);
    void set_bytes(uint32_t address, const std::vector<uint8_t>& bytes);
    void set_bytes(uint32_t address, const uint8_t* bytes, size_t bytes_size);

    [[nodiscard]] const uint8_t* buffer_pointer() const { return &((*this)[0]); }
    [[nodiscard]] uint8_t* buffer_pointer() { return &((*this)[0]); }
private:
    static uint16_t convert_word_to_little_endian(uint16_t big_endian_word);
    static uint32_t convert_long_to_little_endian(uint32_t big_endian_long);
};