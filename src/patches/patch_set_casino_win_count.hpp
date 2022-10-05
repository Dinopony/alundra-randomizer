#pragma once

#include "game_patch.hpp"
#include "../constants/map_codes.hpp"
#include "../constants/flags.hpp"

class PatchSetCasinoWinCount : public GamePatch
{
private:
    uint8_t _win_count_for_second_reward;

public:
    explicit PatchSetCasinoWinCount(uint8_t win_count) : _win_count_for_second_reward(win_count)
    {}

    void alter_datas_file(BinaryFile& data, const GameData& game_data, const RandomizerWorld& world) override
    {
        if(_win_count_for_second_reward == 1)
        {
            data.set_word_le(0x65978E8, FLAG_CASINO_SLIME_1_WIN.event_code());
            data.set_word_le(0x65C7A6E, FLAG_CASINO_SHOOTING_RANGE_1_WIN.event_code());
            data.set_word_le(0x65FD0B0, FLAG_CASINO_ROULETTE_1_WIN.event_code());
        }
        else if(_win_count_for_second_reward == 2)
        {
            data.set_word_le(0x65978E8, FLAG_CASINO_SLIME_2_WINS.event_code());
            data.set_word_le(0x65C7A6E, FLAG_CASINO_SHOOTING_RANGE_2_WINS.event_code());
            data.set_word_le(0x65FD0B0, FLAG_CASINO_ROULETTE_2_WINS.event_code());
        }
        else if(_win_count_for_second_reward == 3)
        {
            data.set_word_le(0x65978E8, FLAG_CASINO_SLIME_3_WINS.event_code());
            data.set_word_le(0x65C7A6E, FLAG_CASINO_SHOOTING_RANGE_3_WINS.event_code());
            data.set_word_le(0x65FD0B0, FLAG_CASINO_ROULETTE_3_WINS.event_code());
        }
        // else, don't change
    }
};
