#pragma once

#include "game_patch.hpp"
#include "../tools/byte_array.hpp"
#include "../tools/mips_tools.hpp"

constexpr uint32_t CODE_BASE_ADDR = 0x1F800;
constexpr uint16_t MAP_JESS_HOUSE_RONAN_VERSION = 0x00F2;

class PatchNewGame : public GamePatch
{
private:
    bool _megaliths_enabled_on_start;

public:
    explicit PatchNewGame(bool megaliths_enabled_on_start) :
        _megaliths_enabled_on_start(megaliths_enabled_on_start)
    {}

    void alter_exe(PsxExeFile& exe) override
    {
        // Change spawn map ID
        exe.set_word_le(0x12558, MAP_JESS_HOUSE_RONAN_VERSION);
        // Change spawn coords
        exe.set_word_le(0x12564, 0x0026);
        exe.set_word_le(0x12570, 0x000C);

        // Inject and call a custom "init game" function setting up flags properly
        uint32_t return_addr = 0x1253c + CODE_BASE_ADDR;
        uint32_t func_addr = inject_func_init_game(exe, return_addr);
        exe.set_code(0x12534, MipsCode().j(func_addr, false));
    }

private:
    uint32_t inject_func_init_game(PsxExeFile& exe, uint32_t return_addr) const
    {
        MipsCode func;
        {
            // Add instructions replaced by the J injection
            func.add_long(0x680f020c);
            func.add_long(0x00000000);

            func.lui(reg_AT, 0x801E);

            // Mark HUD as allowed (0x40 -> 0x1DD403)
            func.li(reg_T0, 0x0040);
            func.sb(reg_T0, reg_AT, 0xD403);

            // Remove Septimus cutscene telling us Ronan has gone insane (0x01 -> 0x1DD358)
            func.li(reg_T0, 0x0001);
            func.sb(reg_T0, reg_AT, 0xD358);
            // Remove Sybill cutscene showing Holy Sword cutscene (0x60 -> 0x1DD3F3)
            func.li(reg_T0, 0x0060);
            func.sb(reg_T0, reg_AT, 0xD3F3);

            // Set default Inoa version to have Lutas chimney open (0x00F1 -> 0x1DD578 && 0x1DD586)
            func.li(reg_T0, 0x00F1);
            func.sh(reg_T0, reg_AT, 0xD578);
            func.sh(reg_T0, reg_AT, 0xD586);

            // Open Riverside bar (0x08 -> 0x1DD385)
            func.li(reg_T0, 0x0008);
            func.sb(reg_T0, reg_AT, 0xD385);

            // Remove the fallen tree on the way to Cliffs of Madness (0x20 -> 0x1DD35F)
            func.li(reg_T0, 0x0020);
            func.sb(reg_T0, reg_AT, 0xD35F);
            // Change the sign on the way to Cliffs of Madness to remove the fallen tree warning (0x08 -> 0x1DD34F)
            func.li(reg_T0, 0x0008);
            func.sb(reg_T0, reg_AT, 0xD34F);

            if(_megaliths_enabled_on_start)
            {
                // Activate megaliths (0x01 -> 0x1DD3E6)
                func.li(reg_T0, 0x0001);
                func.sb(reg_T0, reg_AT, 0xD3E6);
            }

            // Jump back to calling procedure
            func.j(return_addr);
        }
        return exe.inject_code(func);
    }
};