#pragma once

#include <cstdint>

class Flag {
private:
    uint16_t _offset;
    uint8_t _bit;

public:
    constexpr Flag(uint16_t offset, uint8_t bit) : _offset(offset), _bit(bit)
    {}

    [[nodiscard]] uint16_t offset() const { return _offset; }
    [[nodiscard]] uint16_t bit() const { return _bit; }
};