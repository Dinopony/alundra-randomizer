#pragma once

#include <stdexcept>

class RandomizerException : public std::exception {
public:
    explicit RandomizerException(const std::string& reason) : std::exception(reason.c_str())
    {}
};