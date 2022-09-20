#pragma once

#include "game_patch.hpp"
#include "../tools/byte_array.hpp"

/**
 * In the vanilla game, having max HP / MP prevents form using consumables meant to refill the
 * corresponding resource. This patch removes this condition, to prevent some softlocks where
 * you are forced to open a chest in order to proceed, but sometimes are stuck because you
 * cannot use those without any way to lower the corresponding resource.
 */
class PatchAllowUsingConsumablesWhileFull : public GamePatch
{
public:
    void alter_exe_file(PsxExeFile& exe, const GameData& game_data, const RandomizerWorld& world) override
    {
        // Herbs
        exe.set_code(0x15658, MipsCode().nop());

        // Strength Tonic
        exe.set_code(0x15844, MipsCode().nop());

        // Strength Elixyr
        exe.set_code(0x156C8, MipsCode().nop());

        // Magic Elixyr
        exe.set_code(0x1573C, MipsCode().nop());

        // Wonder Essence
        exe.set_code(0x157B0, MipsCode().li(reg_V0, 0x1));
    }
};
