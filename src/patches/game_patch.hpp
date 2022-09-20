#pragma once

#include "../tools/binary_file.hpp"
#include "../tools/psx_exe_file.hpp"
#include "../game/game_data.hpp"
#include "../model/randomizer_world.hpp"

class GamePatch
{
public:
    GamePatch() = default;

    virtual void alter_game_data(GameData& game_data) {}

    virtual void alter_datas_file(BinaryFile& data_file, const GameData& game_data, const RandomizerWorld& world) {}

    virtual void alter_exe_file(PsxExeFile& exe_file, const GameData& game_data, const RandomizerWorld& world) {}
};