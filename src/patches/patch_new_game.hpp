#pragma once

#include "game_patch.hpp"
#include "../tools/byte_array.hpp"
#include "../tools/mips_tools.hpp"
#include "../constants/map_codes.hpp"

class PatchNewGame : public GamePatch
{
private:
    static constexpr uint32_t CODE_BASE_ADDR = 0x1F800;

    const RandomizerOptions& _options;

public:
    explicit PatchNewGame(const RandomizerOptions& options) : _options(options)
    {}

    void alter_exe_file(PsxExeFile& exe, const GameData& game_data, const RandomizerWorld& world) override
    {
        // Change spawn map ID
        exe.set_word_le(0x12558, MAP_JESS_HOUSE_RANDOMIZER);
        // Change spawn coords
        exe.set_word_le(0x12564, 0x0026);
        exe.set_word_le(0x12570, 0x000C);

        // Inject and call a custom "init game" function setting up flags properly
        uint32_t return_addr = 0x1253c + CODE_BASE_ADDR;
        uint32_t func_addr = inject_func_init_game(exe, return_addr, game_data);
        exe.set_code(0x12534, MipsCode().j(func_addr, false));

        // Set starting HP
        exe.set_code(0x12590, MipsCode().addiu(reg_A0, reg_ZERO, _options.starting_health()));
        exe.set_code(0x12598, MipsCode().addiu(reg_A0, reg_ZERO, _options.starting_health()));
        // Set starting MP
        exe.set_code(0x125A0, MipsCode().addiu(reg_A0, reg_ZERO, _options.starting_mana()));
        exe.set_code(0x125A8, MipsCode().addiu(reg_A0, reg_ZERO, _options.starting_mana()));
        // Set starting Gilders
        exe.set_code(0x125B0, MipsCode().addiu(reg_A0, reg_ZERO, _options.starting_gold()));

        // Set starting items
        std::vector<bool> items_owned_on_start;
        items_owned_on_start.resize(ITEM_COUNT, false);
        for(const Item* item : world.starting_inventory())
            items_owned_on_start[item->id()] = true;

        for(uint8_t item_id = 1 ; item_id < ITEM_COUNT ; ++item_id)
        {
            uint32_t item_info_2_addr = 0xA85EA + (item_id * 8);
            uint8_t flag = exe.get_byte(item_info_2_addr) & 0x7F;
            if(items_owned_on_start[item_id])
                flag |= 0x80;

            exe.set_byte(item_info_2_addr, flag);
        }
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
            func.jal(0x83DA0);

            func.lui(reg_AT, 0x801E);

            // For each non-null flag word, add instructions to set it up on new game
            for(size_t addr = 0 ; addr < flag_bytes.size() ; addr += 0x2)
            {
                uint16_t value = flag_bytes.get_word_le(addr);
                if(value == 0)
                    continue;

                func.set_(reg_T0, value);
                func.sh(reg_T0, reg_AT, addr + 0xD000);
            }

            // Set default Inoa version to have Lutas chimney open (0x00F1 -> 0x1DD578 && 0x1DD586)
            func.set_(reg_T0, MAP_INOA_RANDOMIZER);
            func.sh(reg_T0, reg_AT, 0xD578);
            func.sh(reg_T0, reg_AT, 0xD586);
            func.sh(reg_T0, reg_AT, 0xD63A); // 0xF1 -> 0x1DD63A for variant where blue chest can be opened

            // Set Jess house to a variant where chest can be opened
            func.set_(reg_T0, MAP_JESS_HOUSE_CHEST_OPENABLE);
            func.sh(reg_T0, reg_AT, 0xD618);

            // Set Overworld C4 to the "Merrick shop open" variant (0x0197 -> 0x1DD44C)
            func.set_(reg_T0, MAP_OVERWORLD_C4_MERRICK_OPEN_VARIANT);
            func.sh(reg_T0, reg_AT, 0xD44C);

            // Set Overworld E1 (Nirude) to a variant where things work properly
            func.set_(reg_T0, 0x01B7);
            func.sh(reg_T0, reg_AT, 0xD456);

            /*
             * In the vanilla game, only a handful of items can be found in shops and be taken inside Alundra's hands.
             * Developers only set valid physics properties to item objects that could be encountered inside shops,
             * not to every possible item object. This means, if we want any item to work inside shops, we need to copy
             * the "good" physics properties from a well-handled object to all other item objects. This code chunk does this.
             */
            func.set_(reg_T5, 0x800FEFB4);
            func.set_(reg_T6, 0x800FEFFC);
            func.set_(reg_AT, 0x800CC004);
            func.set_(reg_T4, 0x800CC168);
            func.label_("entity_type_def_loop");
            {
                func.lw(reg_T3, reg_AT);
                func.beq_(reg_T3, 0xFFFFFFFF, "next_iter");
                {
                    func.sw(reg_T5, reg_T3);
                    func.sw(reg_T6, reg_T3, 4);
                }
                func.label_("next_iter");
                func.addiu(reg_AT, 0x4);
                func.blt_(reg_AT, reg_T4, "entity_type_def_loop");
            }

            // Jump back to calling procedure
            func.j(return_addr);
        }
        return exe.inject_code(func);
    }
};
