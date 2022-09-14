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

    void add_byte(uint8_t byte)
    { 
        this->emplace_back(byte);
    }
    void add_word(uint16_t word)
    {
        uint8_t msb = word >> 8;
        uint8_t lsb = word & 0x00FF;
        this->add_byte(msb);
        this->add_byte(lsb);
    }
    void add_long(uint32_t long_word)
    {
        uint16_t msw = long_word >> 16;
        uint16_t lsw = long_word & 0x0000FFFF;
        this->add_word(msw);
        this->add_word(lsw);
    }

    void add_bytes(const std::vector<uint8_t>& bytes)
    {
        this->insert(this->end(), bytes.begin(), bytes.end());
    }

    // Little endian functions
    void add_word_le(uint16_t word)
    {
        uint16_t le_word = 0x0000;
        le_word |= (word & 0xFF00) >> 8;
        le_word |= (word & 0x00FF) << 8;
        this->add_word(le_word);
    }
    void add_long_le(uint32_t long_word)
    {
        uint32_t le_long = 0x00000000;
        le_long |= (long_word & 0xFF000000) >> 24;
        le_long |= (long_word & 0x00FF0000) >> 8;
        le_long |= (long_word & 0x0000FF00) << 8;
        le_long |= (long_word & 0x000000FF) << 24;
        this->add_long(le_long);
    }

    [[nodiscard]] uint8_t  byte_at(size_t offset) const { return this->at(offset); }
    [[nodiscard]] uint16_t word_at(size_t offset) const { return ((uint16_t)this->byte_at(offset) << 8) + this->byte_at(offset+1); }
    [[nodiscard]] uint32_t long_at(size_t offset) const { return ((uint32_t)this->word_at(offset) << 16) + this->word_at(offset+2); }

    void byte_at(size_t offset, uint8_t value) { (*this)[offset] = value; }
    void word_at(size_t offset, uint16_t value)
    {
        this->byte_at(offset, value >> 8);
        this->byte_at(offset+1, value & 0xFF);
    }
    void long_at(size_t offset, uint32_t value)
    {
        this->word_at(offset, value >> 16);
        this->word_at(offset+2, value & 0xFFFF);
    }

    void remove_byte(size_t offset) { this->erase(this->begin() + (int32_t)offset); }
    void remove_word(size_t offset) { this->remove_byte(offset); this->remove_byte(offset); }
    void remove_long(size_t offset) { this->remove_word(offset); this->remove_word(offset); }

    [[nodiscard]] uint8_t  last_byte() const { return this->byte_at(this->size()-1); }
    [[nodiscard]] uint16_t last_word() const { return this->word_at(this->size()-2); }
    [[nodiscard]] uint32_t last_long() const { return this->long_at(this->size()-4); }
};