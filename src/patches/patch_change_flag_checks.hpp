#pragma once

#include "game_patch.hpp"
#include "../tools/byte_array.hpp"

class PatchChangeFlagChecks : public GamePatch
{
public:
    void alter_data(BinaryFile& data, const World& world) override
    {
        // Change the condition to open Riverside Bar
        data.set_word_le(0x29B3A6, 0x067E);
        data.set_word_le(0x29AF35, 0x067E);
        data.set_word_le(0x29B5E5, 0x067E);
    }
};

