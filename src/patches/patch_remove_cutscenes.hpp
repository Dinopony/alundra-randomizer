#pragma once

#include "game_patch.hpp"

class PatchRemoveCutscenes : public GamePatch {
public:
    void alter_datas_file(BinaryFile& data, const GameData& game_data, const RandomizerWorld& world) override
    {
        // Remove cutscene between Dark Dragon and Melzas 1
        constexpr uint32_t WARP_TO_SANCTUARY_CUTSCENE_ADDR = 0x47C9E15;
        constexpr uint32_t WARP_TO_MELZAS_1_ADDR = 0x67C51C7;
        ByteArray warp_to_melzas_1_instr =  data.get_bytes(WARP_TO_MELZAS_1_ADDR, WARP_TO_MELZAS_1_ADDR + 8);
        data.set_bytes(WARP_TO_SANCTUARY_CUTSCENE_ADDR, warp_to_melzas_1_instr);
    }
};
