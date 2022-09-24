#pragma once

#include "game_patch.hpp"

/**
 * In vanilla game, rare items trigger a fanfare when obtained from a chest.
 * This patch removes this behavior.
 */
class PatchRemoveItemsFanfare : public GamePatch {
public:
    void alter_exe_file(PsxExeFile& exe_file, const GameData& game_data, const RandomizerWorld& world) override
    {
        constexpr uint32_t ITEM_INFO_2_TABLE_START = 0xA85E4;
        for(uint8_t i=0 ; i<ITEM_COUNT ; ++i)
        {
            uint32_t item_address = ITEM_INFO_2_TABLE_START + (i*8);
            // If item was using the fanfare effect (0x00), use the common item effect instead (0x34)
            if(exe_file.get_byte(item_address + 5) == 0x00)
                exe_file.set_byte(item_address + 5, 0x34);
        }
    }
};