#pragma once

#include <string>
#include <vector>

class TextboxFormatter {
private:
    const std::string& _input_string;

    std::string _output_string;
    std::vector<std::string> _words;
    uint16_t _current_line_width = 0;
    uint16_t _line_count = 0;
    bool _previous_word_requires_space = false;

public:
    explicit TextboxFormatter(const std::string& input_string);

    std::string format();

private:
    void find_words();

    void add_dynamic_newline();
    void add_newline();
    void add_wait_for_input_and_newline();
};
