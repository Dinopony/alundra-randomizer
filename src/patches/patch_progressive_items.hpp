#pragma once

#include "game_patch.hpp"
#include "../tools/byte_array.hpp"
#include "../tools/mips_tools.hpp"

class PatchProgressiveItems : public GamePatch
{
private:
    bool _progressive_boots;
public:
    explicit PatchProgressiveItems(bool progressive_boots) : _progressive_boots(progressive_boots)
    {}

    void alter_exe_file(PsxExeFile& exe, const GameData& game_data, const RandomizerWorld& world) override
    {
        // Inject and call a custom "init game" function setting up flags properly
        uint32_t func_addr = inject_alter_item_id(exe);
        exe.set_code(0x303F4, MipsCode().jal(func_addr));
    }

private:
    uint32_t inject_alter_item_id(PsxExeFile& exe) const
    {
        MipsCode func;

        constexpr uint32_t FUN_FindOwnedItemOfBestTierForSlot = 0x4F810;
        constexpr uint32_t ITEM_INFO_TABLE = 0x800CBE50;

        constexpr auto input = reg_A0; // input is item ID
        constexpr auto return_value = reg_V0;

        constexpr auto item_info_offset = reg_T0;
        constexpr auto current_slot = reg_T0;
        constexpr auto current_tier = reg_T0;
        constexpr auto current_item_id = reg_T1;

        constexpr auto input_item_slot = reg_T8;
        constexpr auto best_item_id = reg_T7;
        constexpr auto tier_to_find = reg_T6;

        constexpr auto storage_A0 = reg_T5;
        constexpr auto storage_A1 = reg_T4;
        constexpr auto storage_RA = reg_T3;

        func.set_(storage_A0, reg_A0);
        func.set_(storage_A1, reg_A1);
        func.set_(storage_RA, reg_RA);

        // if(item_id > ITEM_WIND_BOOK) return
        func.bgt_(reg_A0, ITEM_WIND_BOOK, "return");

        // If we are using "splitBootsEffect" setting, we don't want the boots to be progressive.
        // If it's set, perform this additionnal check to return in case we are processing boots.
        if(!_progressive_boots)
        {
            // if(item_id >= ITEM_SHORT_BOOTS && item_id <= ITEM_CHARM_BOOTS) return
            func.blt_(reg_A0, ITEM_SHORT_BOOTS, "do_process");
            func.bgt_(reg_A0, ITEM_CHARM_BOOTS, "do_process");
            {
                func.bra_("return");
            }
        }

        // Get slot for current item
        func.label_("do_process");
        func.multiply_(item_info_offset, input, 10);        // item_info_offset = input * 10
        func.set_(reg_AT, ITEM_INFO_TABLE);                 // |
        func.addu(reg_AT, reg_AT, item_info_offset);        // |
        func.lh(input_item_slot, reg_AT);                   // input_item_slot_id = *(MAP_INFO_TABLE + item_info_offset)

        // Find the item of best tier for this slot currently owned by the player
        func.set_(input, input_item_slot);                  // |
        func.jal(FUN_FindOwnedItemOfBestTierForSlot);       // |
        func.set_(best_item_id, return_value);              // best_item_id = FUN_FindOwnedItemOfBestTierForSlot(input_item_slot_id)

        func.bgez(best_item_id, "found_best_item");
        // if(best_item_id >= 0)
        {
            // It means we don't own any item for this category
            func.clr_(tier_to_find);                        // tier_to_find = 0
            func.bra_("tier_found");
        }
        // else, if we have an item, tier to find is owned tier + 1
        {
            func.label_("found_best_item");
            func.multiply_(item_info_offset, best_item_id, 10); // item_info_offset = best_item_id * 10
            func.set_(reg_AT, ITEM_INFO_TABLE);                 // |
            func.addu(reg_AT, reg_AT, item_info_offset);        // |
            func.lh(tier_to_find, reg_AT, 4);                   // tier_to_find = *(MAP_INFO_TABLE + item_info_offset + 4)
            func.addiu(tier_to_find, 1);                        // tier_to_find++
        }
        func.label_("tier_found");

        // Try to find an item with slot==input_item_slot_id and tier==tier_to_find
        func.set_(reg_AT, ITEM_INFO_TABLE);
        func.set_(current_item_id, 0);

        // while(current_item_id <= 60)
        func.label_("loop");
        {
            func.lh(current_slot, reg_AT);
            func.bne(current_slot, input_item_slot, "next_iter");
            func.lh(current_tier, reg_AT, 4);
            func.bne(current_tier, tier_to_find, "next_iter");
            // if(current_slot == input_item_slot && current_tier == tier_to_find)
            {
                // If we reach here, it means we found the item we were looking for
                func.set_(reg_A0, current_item_id); // Change the input values for the calling function
                func.set_(reg_A1, current_item_id);
                func.bra_("return");
            }
            // else
            {
                func.label_("next_iter");
                func.addiu(reg_AT, reg_AT, 10);                     // current_addr += 10
                func.addiu(current_item_id, current_item_id, 1);    // current_item_id++
                func.ble_(current_item_id, 60, "loop");             // if(current_item_id > 60) break
            }
        }

        // If we reached here, it means we didn't find any better item, so put back the original object and return
        func.set_(reg_A0, storage_A0);      // |
        func.set_(reg_A1, storage_A1);      // Restore the original input values for the calling function

        func.label_("return");
        func.set_(reg_V0, 0x8013E234);      // Code portion removed to inject the call
        func.lw(reg_V0, reg_V0);            // |
        func.set_(reg_RA, storage_RA);      // |
        func.jr(reg_RA);                    // return;

        return exe.inject_code(func);
    }
};
