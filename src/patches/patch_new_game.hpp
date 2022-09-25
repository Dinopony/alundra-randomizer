#pragma once

#include "game_patch.hpp"
#include "../tools/byte_array.hpp"
#include "../tools/mips_tools.hpp"

constexpr uint32_t CODE_BASE_ADDR = 0x1F800;
constexpr uint16_t MAP_JESS_HOUSE_RONAN_VERSION = 0x00F2;

class PatchNewGame : public GamePatch
{
public:
    void alter_exe_file(PsxExeFile& exe, const GameData& game_data, const RandomizerWorld& world) override
    {
        // Change spawn map ID
        exe.set_word_le(0x12558, MAP_JESS_HOUSE_RONAN_VERSION);
        // Change spawn coords
        exe.set_word_le(0x12564, 0x0026);
        exe.set_word_le(0x12570, 0x000C);

        // Inject and call a custom "init game" function setting up flags properly
        uint32_t return_addr = 0x1253c + CODE_BASE_ADDR;
        uint32_t func_addr = inject_func_init_game(exe, return_addr, game_data);
        exe.set_code(0x12534, MipsCode().j(func_addr, false));
    }

private:
    static uint32_t inject_func_init_game(PsxExeFile& exe, uint32_t return_addr, const GameData& game_data)
    {
        // Build a memory map of starting flags in order to know which
        // words we need to edit
        ByteArray flag_bytes;
        flag_bytes.resize(0x1000, 0);
        for(const Flag& flag : game_data.starting_flags())
        {
            uint16_t mask = 0x0001 << flag.bit();
            flag_bytes[flag.offset() - 0xD000] |= mask;
        }

        // Add the function that set the flags up along with initializing map variants
        MipsCode func;
        {
            // Add instructions replaced by the J injection
            func.add_long(0x680f020c);
            func.add_long(0x00000000);

            func.lui(reg_AT, 0x801E);

            // For each non-null flag word, add instructions to set it up on new game
            for(size_t addr = 0 ; addr < flag_bytes.size() ; addr += 0x2)
            {
                uint16_t value = flag_bytes.get_word_le(addr);
                if(value == 0)
                    continue;

                func.li(reg_T0, value);
                func.sh(reg_T0, reg_AT, addr + 0xD000);
            }

            // Set default Inoa version to have Lutas chimney open (0x00F1 -> 0x1DD578 && 0x1DD586)
            func.li(reg_T0, 0x00F1);
            func.sh(reg_T0, reg_AT, 0xD578);
            func.sh(reg_T0, reg_AT, 0xD586);
            func.sh(reg_T0, reg_AT, 0xD676); // 0xF1 -> 0x1DD676 for post-Torla cutscene

            // Also put 0xF2 -> 0x1DD676 for post-Torla house cutscene at night (where Meia wakes us up to warn
            // that Bergus has gone missing)
            func.li(reg_T0, 0x00F2);
            func.sh(reg_T0, reg_AT, 0xD678);

            // Set Overworld C4 to the "Merrick shop open" variant (0x0197 -> 0x1DD44C)
            func.li(reg_T0, 0x0197);
            func.sh(reg_T0, reg_AT, 0xD44C);

            // Set Overworld E1 (Nirude) to a variant where things work properly
            func.li(reg_T0, 0x01B7);
            func.sh(reg_T0, reg_AT, 0xD456);

            // Jump back to calling procedure
            func.j(return_addr);
        }
        return exe.inject_code(func);
    }
};
