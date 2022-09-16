#pragma once

#include <stdexcept>
#include <string>
#include "byte_array.hpp"

// -------------------- Reading functions --------------------

uint8_t ByteArray::get_byte(uint32_t address) const
{
    return this->at(address);
}

uint16_t ByteArray::get_word(uint32_t address) const
{
    return (static_cast<uint16_t>(this->get_byte(address) << 8))
           + static_cast<uint16_t>(this->get_byte(address + 1));
}

uint32_t ByteArray::get_long(uint32_t address) const
{
    return (static_cast<uint32_t>(this->get_word(address)) << 16)
           + static_cast<uint32_t>(this->get_word(address + 2));
}

uint16_t ByteArray::get_word_le(uint32_t address) const
{
    uint16_t be_word = this->get_word(address);
    uint16_t le_word = 0x0000;
    le_word |= (be_word & 0xFF00) >> 8;
    le_word |= (be_word & 0x00FF) << 8;
    return le_word;
}

uint32_t ByteArray::get_long_le(uint32_t address) const
{
    uint32_t be_long = this->get_long(address);
    uint32_t le_long = 0x00000000;
    le_long |= (be_long & 0xFF000000) >> 24;
    le_long |= (be_long & 0x00FF0000) >> 8;
    le_long |= (be_long & 0x0000FF00) << 8;
    le_long |= (be_long & 0x000000FF) << 24;
    return le_long;
}

ByteArray ByteArray::get_bytes(uint32_t begin, uint32_t end) const
{
    ByteArray output;
    output.reserve(end - begin);

    for (uint32_t addr = begin; addr < end; addr += 0x1)
        output.emplace_back(this->get_byte(addr));

    return output;
}


// -------------------- Add functions --------------------

void ByteArray::add_byte(uint8_t byte)
{
    this->emplace_back(byte);
}

void ByteArray::add_word(uint16_t word)
{
    uint8_t msb = word >> 8;
    uint8_t lsb = word & 0x00FF;
    this->add_byte(msb);
    this->add_byte(lsb);
}

void ByteArray::add_long(uint32_t long_word)
{
    uint16_t msw = long_word >> 16;
    uint16_t lsw = long_word & 0x0000FFFF;
    this->add_word(msw);
    this->add_word(lsw);
}

void ByteArray::add_word_le(uint16_t word)
{
    this->add_word(convert_word_to_little_endian(word));
}

void ByteArray::add_long_le(uint32_t long_word)
{
    this->add_long(convert_long_to_little_endian(long_word));
}

void ByteArray::add_bytes(const std::vector<uint8_t>& bytes)
{
    this->insert(this->end(), bytes.begin(), bytes.end());
}

void ByteArray::add_bytes(const uint8_t* bytes, size_t bytes_size)
{
    for (uint32_t i = 0; i < bytes_size; ++i)
        this->add_byte(bytes[i]);
}


// -------------------- Set functions --------------------

void ByteArray::set_byte(uint32_t address, uint8_t byte)
{
    if (address >= this->size())
        throw std::out_of_range("Offset " + std::to_string(address) + " is out of range from ByteArray");

    (*this)[address] = byte;
}

void ByteArray::set_word(uint32_t address, uint16_t word)
{
    this->set_byte(address, (word >> 8));
    this->set_byte(address + 1, (word & 0xFF));
}

void ByteArray::set_long(uint32_t address, uint32_t long_word)
{
    this->set_word(address, (long_word >> 16));
    this->set_word(address + 2, (long_word & 0xFFFF));
}

void ByteArray::set_word_le(uint32_t address, uint16_t word)
{
    this->set_word(address, convert_word_to_little_endian(word));
}

void ByteArray::set_long_le(uint32_t address, uint32_t long_word)
{
    this->set_long(address, convert_long_to_little_endian(long_word));
}

void ByteArray::set_bytes(uint32_t address, const std::vector<uint8_t>& bytes)
{
    for (uint32_t i = 0; i < bytes.size(); ++i)
    {
        this->set_byte(address + i, bytes[i]);
    }
}

void ByteArray::set_bytes(uint32_t address, const unsigned char* bytes, size_t bytes_size)
{
    for (uint32_t i = 0; i < bytes_size; ++i)
        this->set_byte(address + i, bytes[i]);
}


// -------------------- Utility functions --------------------

uint16_t ByteArray::convert_word_to_little_endian(uint16_t big_endian_word)
{
    uint16_t le_word = 0x0000;
    le_word |= (big_endian_word & 0xFF00) >> 8;
    le_word |= (big_endian_word & 0x00FF) << 8;
    return le_word;
}

uint32_t ByteArray::convert_long_to_little_endian(uint32_t big_endian_long)
{
    uint32_t le_long = 0x00000000;
    le_long |= (big_endian_long & 0xFF000000) >> 24;
    le_long |= (big_endian_long & 0x00FF0000) >> 8;
    le_long |= (big_endian_long & 0x0000FF00) << 8;
    le_long |= (big_endian_long & 0x000000FF) << 24;
    return le_long;
}