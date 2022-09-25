#pragma once

#include "game_patch.hpp"
#include "../tools/byte_array.hpp"

class PatchChangeFlagChecks : public GamePatch
{
public:
    void alter_datas_file(BinaryFile& data, const GameData& game_data, const RandomizerWorld& world) override
    {
        // Change the condition to open Riverside Bar
        data.set_word_le(0x29B3A6, 0x067E);
        data.set_word_le(0x29AF35, 0x067E);
        data.set_word_le(0x29B5E5, 0x067E);

        // Make the bouquet the condition to be able to open Lars Crypt
        data.set_word_le(0x364BFE, 0x3120);
        data.set_word_le(0x494ED32, 0x3120);

        // Remove Nirude having to be broken for Torla geysers to work
        data.set_word_le(0x43F774, 0x0005);
        data.set_word_le(0x43F7DB, 0x0005);
        data.set_word_le(0x43F83F, 0x0005);
        data.set_word_le(0x43F8A3, 0x0005);
        data.set_word_le(0x43F907, 0x0005);
    }
};

