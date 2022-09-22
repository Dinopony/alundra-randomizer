#pragma once

#include "../tools/json.hpp"
#include <string>

class RandomizerWorld;
class RandomizerOptions;
class GameData;

namespace ModelWriter {
    void write_logic_model(const RandomizerWorld& world);
}

namespace SpoilerWriter {
    Json build_spoiler_json(const RandomizerWorld& world, const RandomizerOptions& options);
}

namespace GraphvizWriter {
    void write_logic_as_dot(const RandomizerWorld& world, const std::string& path);
}
