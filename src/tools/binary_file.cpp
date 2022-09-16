#include "binary_file.hpp"
#include "exception.hpp"
#include "byte_array.hpp"

#include <iostream>

BinaryFile::BinaryFile(const std::string& input_path) : _input_path(input_path)
{
    std::ifstream file(input_path, std::ios::binary);
    if (!file.is_open())
        throw RandomizerException("Could not open binary file '" + input_path + "'");

    file.seekg(0, std::ios::end);
    std::streamsize file_size = file.tellg();
    _byte_array.resize(file_size);
    file.seekg(0);
    file.read((char*)&(_byte_array[0]), file_size);
    file.close();
}

BinaryFile::BinaryFile(const std::vector<uint8_t>& bytes)
{
    _byte_array = bytes;
}

BinaryFile::BinaryFile(const uint8_t* buffer, size_t size)
{
    _byte_array.resize(size);
    for(size_t i=0 ; i<size ; ++i)
        _byte_array[i] = buffer[i];
}

void BinaryFile::set_byte(uint32_t address, uint8_t byte)
{
    if (address >= _byte_array.size())
        return;

    _byte_array[address] = byte;
}

void BinaryFile::set_word(uint32_t address, uint16_t word)
{
    if (address >= _byte_array.size() - 1)
        return;

    this->set_byte(address, (word >> 8));
    this->set_byte(address + 1, (word & 0xFF));
}

void BinaryFile::set_long(uint32_t address, uint32_t longWord)
{
    if (address >= _byte_array.size() - 3)
        return;

    this->set_word(address, (longWord >> 16));
    this->set_word(address + 2, (longWord & 0xFFFF));
}

std::vector<uint8_t> BinaryFile::get_bytes(uint32_t begin, uint32_t end) const
{
    std::vector<uint8_t> output;
    output.reserve(end - begin);

    for (uint32_t addr = begin; addr < end; addr += 0x1)
        output.emplace_back(this->get_byte(addr));

    return output;
}

std::vector<uint16_t> BinaryFile::get_words(uint32_t begin, uint32_t end) const
{
    std::vector<uint16_t> output;
    output.reserve(((end - begin) / 0x2) + 1);

    for (uint32_t addr = begin; addr < end; addr += 0x2)
        output.emplace_back(this->get_word(addr));

    return output;
}

std::vector<uint32_t> BinaryFile::get_longs(uint32_t begin, uint32_t end) const
{
    std::vector<uint32_t> output;
    output.reserve(((end - begin) / 0x4) + 1);

    for (uint32_t addr = begin; addr < end; addr += 0x4)
        output.emplace_back(this->get_long(addr));

    return output;
}

void BinaryFile::set_bytes(uint32_t address, const std::vector<uint8_t>& bytes)
{
    for (uint32_t i = 0; i < bytes.size(); ++i)
    {
        this->set_byte(address + i, bytes[i]);
    }
}

void BinaryFile::set_bytes(uint32_t address, const unsigned char* bytes, size_t bytes_size)
{
    for (uint32_t i = 0; i < bytes_size; ++i)
    {
        this->set_byte(address + i, bytes[i]);
    }
}

uint16_t BinaryFile::get_word_le(uint32_t address) const
{
    uint16_t be_word = this->get_word(address);
    uint16_t le_word = 0x0000;
    le_word |= (be_word & 0xFF00) >> 8;
    le_word |= (be_word & 0x00FF) << 8;
    return le_word;
}

uint32_t BinaryFile::get_long_le(uint32_t address) const
{
    uint32_t be_long = this->get_long(address);
    uint32_t le_long = 0x00000000;
    le_long |= (be_long & 0xFF000000) >> 24;
    le_long |= (be_long & 0x00FF0000) >> 8;
    le_long |= (be_long & 0x0000FF00) << 8;
    le_long |= (be_long & 0x000000FF) << 24;
    return le_long;
}

void BinaryFile::set_word_le(uint32_t address, uint16_t word)
{
    uint16_t le_word = 0x0000;
    le_word |= (word & 0xFF00) >> 8;
    le_word |= (word & 0x00FF) << 8;
    this->set_word(address, le_word);
}

void BinaryFile::set_long_le(uint32_t address, uint32_t long_word)
{
    uint32_t le_long = 0x00000000;
    le_long |= (long_word & 0xFF000000) >> 24;
    le_long |= (long_word & 0x00FF0000) >> 8;
    le_long |= (long_word & 0x0000FF00) << 8;
    le_long |= (long_word & 0x000000FF) << 24;
    this->set_long(address, le_long);
}

void BinaryFile::save_as(const std::string& output_path)
{
    std::ofstream output_file(output_path, std::ios::binary);
    if(!output_file)
        throw RandomizerException("Could not output binary file at '" + output_path + "'");

    output_file.write((char*)&(_byte_array[0]), (int32_t)_byte_array.size());
    output_file.close();
}

uint64_t BinaryFile::checksum() const
{
    uint64_t checksum = 0;
    for(uint8_t byte : _byte_array)
        checksum += byte;
    return checksum;
}
