#pragma once

#include <vector>
#include <map>
#include <string>
#include <random>
#include <algorithm>
#include <sstream>
#include <fstream>
#include "json.hpp"

namespace stringtools
{
    inline std::vector<std::string> split(const std::string& str, const std::string& delim)
    {
        std::vector<std::string> tokens;
        size_t prev = 0, pos;
        do {
            pos = str.find(delim, prev);
            if (pos == std::string::npos)
                pos = str.length();
            std::string token = str.substr(prev, pos-prev);
            tokens.emplace_back(token);

            prev = pos + delim.length();
        } while (pos < str.length() && prev < str.length());

        return tokens;
    }

    inline std::string join(const std::vector<std::string>& words, const std::string& junction)
    {
        if(words.empty())
            return "";

        std::string ret = words[0];
        for(size_t i=1 ; i<words.size() ; ++i)
        {
            ret += junction;
            ret += words[i];
        }

        return ret;
    }

    // trim from start (in place)
    inline void ltrim(std::string& s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
            return !std::isspace(ch);
        }));
    }

    // trim from end (in place)
    inline void rtrim(std::string& s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    }

    // trim from both ends (in place)
    inline void trim(std::string& s) {
        ltrim(s);
        rtrim(s);
    }

    inline void to_lower(std::string& s) {
        std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); });
    }

    inline void replace(std::string& str, const std::string& from, const std::string& to)
    {
        size_t start_pos = str.find(from);
        str.replace(start_pos, from.length(), to);
    }
}
