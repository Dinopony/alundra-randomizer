#pragma once

#include "binary_file.hpp"
#include "mips_tools.hpp"

class PsxExeFile : public BinaryFile
{
private:
    static constexpr uint32_t HEADER_SIZE = 0x800;
    static constexpr uint32_t FILE_SIZE_INCREMENT = 0x800;

    uint32_t _initial_code_start_addr;
    std::vector<uint8_t> _prepended_bytes;

public:
    explicit PsxExeFile(const std::string& input_path) : BinaryFile(input_path)
    {
        _initial_code_start_addr = this->get_long_le(0x18);

        // Add a full empty increment to let an open RAM space for the game to edit
        _prepended_bytes.resize(FILE_SIZE_INCREMENT, 0x0000);
    }

    void save_as(const std::string& output_path) override
    {
        // First, update the PSX-EXE header to expand the file
        this->update_psx_header();

        // Once updated, we can save the file as usual
        BinaryFile::save_as(output_path);
    }

    uint32_t inject_code(const MipsCode& code)
    {
        _prepended_bytes.insert(_prepended_bytes.begin(), code.begin(), code.end());
        uint32_t effective_addr = _initial_code_start_addr - _prepended_bytes.size();
        return effective_addr;
    }

    void set_code(uint32_t addr, const MipsCode& code)
    {
        this->set_bytes(addr, code);
    }

    void update_psx_header()
    {
        uint32_t expansion_size = 0;
        while(expansion_size < _prepended_bytes.size())
            expansion_size += FILE_SIZE_INCREMENT;

        // Prepend padding bytes to ensure the size expansion is a multiple of 0x800
        uint32_t padding_size = expansion_size - _prepended_bytes.size();
        std::vector<uint8_t> empty_bytes;
        empty_bytes.resize(padding_size, 0xFF);
        _prepended_bytes.insert(_prepended_bytes.begin(), empty_bytes.begin(), empty_bytes.end());

        // Actually insert the bytes inside the data
        _byte_array.insert(_byte_array.begin() + HEADER_SIZE, _prepended_bytes.begin(), _prepended_bytes.end());

        if(expansion_size > 0)
        {
            // Shift the address where code is loaded
            uint32_t new_code_loading_addr = _initial_code_start_addr - expansion_size;
            this->set_long_le(0x18, new_code_loading_addr);

            // Update the amount of bytes to load
            this->set_long_le(0x1C, _byte_array.size() - HEADER_SIZE);
        }
    }
};