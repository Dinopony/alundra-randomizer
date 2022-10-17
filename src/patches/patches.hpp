#pragma once

#include <vector>

class RandomizerOptions;
class PersonalSettings;
class GamePatch;
class BinaryFile;
class PsxExeFile;
class GameData;
class RandomizerWorld;

void execute_patches(const std::vector<GamePatch*>& patches, 
                     BinaryFile& data_file, PsxExeFile& exe_file,
                     GameData& game_data, RandomizerWorld& world);

void apply_randomizer_patches(BinaryFile& data, PsxExeFile& exe,
                              GameData& game_data, RandomizerWorld& world,
                              const RandomizerOptions& options, const PersonalSettings& personal_settings);
