#pragma once

#include "game_patch.hpp"

class World;
class RandomizerOptions;

void execute_patches(const std::vector<GamePatch*>& patches, BinaryFile& data, PsxExeFile& exe, World& world);

void apply_randomizer_patches(BinaryFile& data, PsxExeFile& exe, World& world, RandomizerOptions& options);
