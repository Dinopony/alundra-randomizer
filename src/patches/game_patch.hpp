#pragma once

#include "../tools/binary_file.hpp"
#include "../tools/psx_exe_file.hpp"
#include "../model/world.hpp"

class GamePatch
{
public:
    GamePatch() = default;

    virtual void alter_world(World& world) {}

    virtual void alter_data(BinaryFile& data, const World& world) {}

    virtual void alter_exe(PsxExeFile& exe, const World& world) {}
};