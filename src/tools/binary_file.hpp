#pragma once

#include <string>
#include <vector>
#include <map>
#include <fstream>

#ifdef DEBUG
#include <set>
#endif

class BinaryFile
{
protected:
    std::string _input_path = "./file.bin";
    std::vector<uint8_t> _byte_array;
    std::map<std::string, uint32_t> _stored_addresses;

public:
    explicit BinaryFile(const std::string& input_path);
    explicit BinaryFile(const std::vector<uint8_t>& bytes);
    BinaryFile(const uint8_t* buffer, size_t size);

    [[nodiscard]] uint8_t get_byte(uint32_t address) const { return _byte_array[address]; }
    [[nodiscard]] uint16_t get_word(uint32_t address) const { return (this->get_byte(address) << 8) + this->get_byte(address+1); }
    [[nodiscard]] uint32_t get_long(uint32_t address) const { return (static_cast<uint32_t>(this->get_word(address)) << 16) + static_cast<uint32_t>(this->get_word(address+2)); }

    [[nodiscard]] std::vector<uint8_t> get_bytes() const { return _byte_array; }
    [[nodiscard]] std::vector<uint8_t> get_bytes(uint32_t begin, uint32_t end) const;
    [[nodiscard]] std::vector<uint16_t> get_words(uint32_t begin, uint32_t end) const;
    [[nodiscard]] std::vector<uint32_t> get_longs(uint32_t begin, uint32_t end) const;

    void set_byte(uint32_t address, uint8_t byte);
    void set_word(uint32_t address, uint16_t word);
    void set_long(uint32_t address, uint32_t long_word);
    void set_bytes(uint32_t address, const std::vector<uint8_t>& bytes);
    void set_bytes(uint32_t address, const unsigned char* bytes, size_t bytes_size);

    // Little endian functions
    [[nodiscard]] uint16_t get_word_le(uint32_t address) const;
    [[nodiscard]] uint32_t get_long_le(uint32_t address) const;
    void set_word_le(uint32_t address, uint16_t word);
    void set_long_le(uint32_t address, uint32_t long_word);

    [[nodiscard]] const uint8_t* iterator_at(uint32_t addr) const { return reinterpret_cast<const uint8_t*>(&(_byte_array[addr])); }

    void store_address(const std::string& name, uint32_t address) { _stored_addresses[name] = address; }
    uint32_t stored_address(const std::string& name) { return _stored_addresses.at(name); }

    void save() { this->save_as(_input_path); }
    virtual void save_as(const std::string& output_path);

    [[nodiscard]] uint32_t size() const { return _byte_array.size(); }

    [[nodiscard]] uint64_t checksum() const;
};
