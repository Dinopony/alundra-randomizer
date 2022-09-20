#pragma once

#include <random>
#include <vector>
#include <initializer_list>
#include <algorithm>

namespace vectools {

    template<typename T>
    inline void erase_first(std::vector<T>& vector, const T& elem)
    {
        auto iter = std::find(vector.begin(), vector.end(), elem);
        if(iter != vector.end())
            vector.erase(iter);
    }

    template<typename T>
    [[nodiscard]] inline bool contains(const std::vector<T>& vector, const T& elem)
    {
        auto iter = std::find(vector.begin(), vector.end(), elem);
        return iter != vector.end();
    }

    template<typename T>
    [[nodiscard]] inline size_t count(const std::vector<T>& vector, const T& elem)
    {
        size_t count = 0;
        for(size_t i=0 ; i<vector.size() ; ++i)
            if(vector[i] == elem)
                count += 1;

        return count;
    }

    template<typename T>
    inline void shuffle(std::vector<T>& vector, std::mt19937& rng)
    {
        if (vector.empty())
            return;

        auto get_uniform_integer = [](uint32_t number, uint32_t range_low, uint32_t range_high) -> uint32_t
        {
            double proportion = static_cast<double>(number) / (1.0 + UINT32_MAX);
            uint32_t range = range_high - range_low + 1;
            return static_cast<uint32_t>(proportion * range) + range_low;
        };

        std::vector<T> elems;

        while (vector.size() > 1)
        {
            uint32_t random_pos = get_uniform_integer(rng(), 0, static_cast<uint32_t>(vector.size()-1));
            elems.emplace_back(vector[random_pos]);
            vector.erase(vector.begin() + random_pos);
        }

        elems.emplace_back(*vector.begin());
        vector = elems;
    }

} // namespace vectools end
