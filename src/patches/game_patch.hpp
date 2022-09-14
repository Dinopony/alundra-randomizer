#pragma once

#include "../tools/binary_file.hpp"
#include "../tools/psxexe_file.hpp"
#include "../model/world.hpp"

class GamePatch
{
public:
    GamePatch() = default;

    virtual void alter_world(World& world) {}

    virtual void alter_data(BinaryFile& data, const World& world) {}

    virtual void alter_exe(PsxExeFile& exe) {}


};