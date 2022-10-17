#pragma once

#include "game_patch.hpp"

/**
 * This patch removes in-game music while leaving SFX intact.
 */
class PatchRemoveMusic : public GamePatch {
public:
    void alter_exe_file(PsxExeFile& exe, const GameData& game_data, const RandomizerWorld& world) override
    {
        exe.set_code(0x2B67C, MipsCode().j(0x4AED4));
    }
};
