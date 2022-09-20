#include "binary_file.hpp"
#include "exception.hpp"
#include "byte_array.hpp"

#include <iostream>

BinaryFile::BinaryFile(const std::filesystem::path& input_path) :
    ByteArray   (),
    _input_path (input_path)
{
    std::ifstream file(input_path, std::ios::binary);
    if (!file.is_open())
        throw RandomizerException("Could not open binary file '" + input_path.string() + "'");

    file.seekg(0, std::ios::end);
    std::streamsize file_size = file.tellg();
    this->resize(file_size);
    file.seekg(0);
    file.read((char*)this->buffer_pointer(), file_size);
    file.close();
}

BinaryFile::BinaryFile(const std::vector<uint8_t>& bytes)
{
    this->resize(bytes.size());
    this->set_bytes(0, bytes);
}

BinaryFile::BinaryFile(const uint8_t* buffer, size_t size)
{
    this->resize(size);
    this->set_bytes(0, buffer, size);
}

void BinaryFile::save_as(const std::filesystem::path& output_path)
{
    std::ofstream output_file(output_path, std::ios::binary);
    if(!output_file)
        throw RandomizerException("Could not output binary file at '" + output_path.string() + "'");

    output_file.write((char*)this->buffer_pointer(), (int32_t)this->size());
    output_file.close();
}

uint64_t BinaryFile::checksum() const
{
    uint64_t checksum = 0;
    for(uint8_t byte : *this)
        checksum += byte;
    return checksum;
}
