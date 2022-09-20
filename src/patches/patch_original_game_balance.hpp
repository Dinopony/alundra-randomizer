#pragma once

#include "game_patch.hpp"
#include "../tools/byte_array.hpp"
#include "../assets/patched_balance.bin.hxx"

class PatchOriginalGameBalance : public GamePatch
{
public:
    void alter_datas_file(BinaryFile& data, const GameData& game_data, const RandomizerWorld& world) override
    {
        BinaryFile balance_file(PATCHED_BALANCE, PATCHED_BALANCE_SIZE);
        balance_file.save_as("./tmp_dump/DATA/BALANCE.BIN");
    }
};
