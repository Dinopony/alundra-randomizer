#pragma once

#include <stdexcept>

class RandomizerException : public std::exception {
private:
    std::string _reason;

public:
    explicit RandomizerException(const std::string& reason) :
        std::exception  (),
        _reason         (reason)
    {}

    [[nodiscard]] const char* what() const noexcept override
    {
        return _reason.c_str();
    }
};
