#include "textbox_formatter.hpp"
#include "stringtools.hpp"

#include <array>

constexpr uint16_t TEXTBOX_WIDTH = 500;
constexpr uint16_t TEXTBOX_LINES = 3;

static const std::array<uint8_t, 256>& get_character_sizes()
{
    constexpr uint8_t DEFAULT_CHARACTER_WIDTH = 14;

    static std::array<uint8_t, 256> character_sizes;

    static bool initialized = false;
    if(!initialized)
    {
        character_sizes.fill(DEFAULT_CHARACTER_WIDTH);
        character_sizes['w'] = 24;
        character_sizes['m'] = 24;
        character_sizes['n'] = 18;
        character_sizes['H'] = 18;
        character_sizes['h'] = 16;
        character_sizes['a'] = 16;
        character_sizes['A'] = 16;
        character_sizes['p'] = 16;
        character_sizes['i'] = 10;
        character_sizes['I'] = 10;
        character_sizes['t'] = 10;
        character_sizes['l'] = 10;
        character_sizes[' '] = 10;
        character_sizes['!'] = 8;
        character_sizes['.'] = 6;
        character_sizes[','] = 6;
        character_sizes['\\'] = 0;
    }

    return character_sizes;
}

TextboxFormatter::TextboxFormatter(const std::string& input_string) :
    _input_string(input_string)
{
    _output_string.reserve(_input_string.size());
}


std::string TextboxFormatter::format()
{
    if(!_output_string.empty())
        return _output_string;

    const std::array<uint8_t, 256>& CHARACTER_SIZES = get_character_sizes();

    this->find_words();

    for(std::string word : _words)
    {
        if(word.starts_with("\\"))
        {
            if(word[1] == 'N')
                this->add_dynamic_newline();
            else if(word[1] == 'A')
                this->add_wait_for_input_and_newline();
            else
                _output_string += word;

            _previous_word_requires_space = false;
            continue;
        }

        uint16_t word_length = 0;
        for(char c : word)
            word_length += CHARACTER_SIZES[c];

        uint16_t word_length_with_space = word_length;
        if(_previous_word_requires_space)
            word_length_with_space += CHARACTER_SIZES[' '];

        if(_current_line_width + word_length_with_space <= TEXTBOX_WIDTH)
        {
            if(_previous_word_requires_space)
                _output_string += " ";
            _output_string += word;

            _current_line_width += word_length_with_space;
            _previous_word_requires_space = true;
        }
        else
        {
            this->add_dynamic_newline();
            _output_string += word;
            _current_line_width += word_length;
            _previous_word_requires_space = true;
        }
    }

    return _output_string;
}

void TextboxFormatter::find_words()
{
    std::string current_word;
    for(size_t i=0 ; i<_input_string.size() ; ++i)
    {
        char c = _input_string[i];
        // Cut words when encountering a space or an escape sequence
        if(!current_word.empty() && (c == ' ' || c == '\\'))
        {
            _words.emplace_back(current_word);
            current_word = "";
        }

        // Process escape sequences as individual words
        if(c == '\\')
        {
            current_word += "\\";
            current_word += _input_string[++i];

            // Wait (\W) instructions carry a digit as an extra argument
            if(current_word == "\\W")
                current_word += _input_string[++i];

            // Ask (\0) instructions are closed by a '\Y'
            if(current_word == "\\0")
            {
                while(_input_string[++i] != '\\')
                    current_word += _input_string[i];

                // Add the trailing `\Y`
                current_word += _input_string[i];
                current_word += _input_string[++i];
            }

            _words.emplace_back(current_word);
            current_word = "";
            continue;
        }
        else if(c == ' ')
            continue;

        current_word += c;
    }

    if(!current_word.empty())
        _words.emplace_back(current_word);
}

void TextboxFormatter::add_dynamic_newline()
{
    // Add a "wait for input and newline" character if we reached 3 lines, or just a newline if we have room for more lines
    if(_line_count < TEXTBOX_LINES-1)
        this->add_newline();
    else
        this->add_wait_for_input_and_newline();
}

void TextboxFormatter::add_newline()
{
    _output_string += "\\N";
    ++_line_count;
    _current_line_width = 0;
}

void TextboxFormatter::add_wait_for_input_and_newline()
{
    _output_string += "\\A";
    _line_count = 0;
    _current_line_width = 0;
}