#pragma once

#include "game_patch.hpp"
#include "../tools/byte_array.hpp"

class PatchSplitBootsEffects : public GamePatch
{
public:
    void alter_exe_file(PsxExeFile& exe, const GameData& game_data, const RandomizerWorld& world) override
    {
        // Change the check for Charm Boots
        MipsCode check_charm_boots;
        check_charm_boots.lui(reg_V0, 0x801E);
        check_charm_boots.lw(reg_V0, reg_V0, 0xD8A0);
        exe.set_code(0x128E8, check_charm_boots);
        exe.set_byte(0x128F4, 0x1);

        // Change the jump check for Long Boots
        MipsCode check_long_boots_jump;
        check_long_boots_jump.lui(reg_V0, 0x801E);
        check_long_boots_jump.lw(reg_V0, reg_V0, 0xD898);
        exe.set_code(0x1790C, check_long_boots_jump);

        // Change the speed check for Long Boots
        MipsCode check_long_boots_speed;
        check_long_boots_speed.lui(reg_V0, 0x801E);
        check_long_boots_speed.lw(reg_V0, reg_V0, 0xD898);
        exe.set_code(0x17A68, check_long_boots_speed);

        // Change the check for Merman Boots
        MipsCode check_merman_boots;
        check_merman_boots.lui(reg_V0, 0x801E);
        check_merman_boots.lw(reg_V0, reg_V0, 0xD89C);
        exe.set_code(0x185C4, check_merman_boots);
        exe.set_byte(0x185D4, 0x1);
    }
};
