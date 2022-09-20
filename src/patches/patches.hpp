#pragma once

#include "game_patch.hpp"

class World;
class RandomizerOptions;

void execute_patches(const std::vector<GamePatch*>& patches, 
                     BinaryFile& data_file, PsxExeFile& exe_file, 
                     GameData& game_data, RandomizerWorld& world);

void apply_randomizer_patches(BinaryFile& data, PsxExeFile& exe, 
                              GameData& game_data, RandomizerWorld& world,
                              const RandomizerOptions& options);
