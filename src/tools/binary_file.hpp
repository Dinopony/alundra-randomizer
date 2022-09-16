#pragma once

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include "byte_array.hpp"

class BinaryFile : public ByteArray
{
protected:
    std::string _input_path = "./file.bin";
    std::map<std::string, uint32_t> _stored_addresses;

public:
    explicit BinaryFile(const std::string& input_path);
    explicit BinaryFile(const std::vector<uint8_t>& bytes);
    BinaryFile(const uint8_t* buffer, size_t size);

    void store_address(const std::string& name, uint32_t address) { _stored_addresses[name] = address; }
    uint32_t stored_address(const std::string& name) { return _stored_addresses.at(name); }

    void save() { this->save_as(_input_path); }
    virtual void save_as(const std::string& output_path);

    [[nodiscard]] uint64_t checksum() const;
};
