cmake_minimum_required(VERSION 3.21)

project(alundra-randomizer VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_definitions(RELEASE="${PROJECT_VERSION}")
add_compile_definitions(MAJOR_RELEASE=${PROJECT_VERSION_MAJOR}${PROJECT_VERSION_MINOR})

if(DEBUG)
    add_compile_definitions(DEBUG)
endif()

# add_subdirectory(extlibs/landstalker_lib landstalker_lib)
include_directories("extlibs/")

macro(WRAPPED_DEPENDENCY)
    message(STATUS "Defining wrapped dependency for ${CMAKE_CURRENT_SOURCE_DIR}/${ARGV1}")
    add_custom_command(
            OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/${ARGV1}.hxx"
            COMMAND "${CMAKE_COMMAND}"
            ARGS -DINPUT_FILE="${ARGV1}" -DVARIABLE_NAME="${ARGV2}" -DPROCESS_TYPE="${ARGV0}" -P "wrap_file.cmake"
            DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${ARGV1}"
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    )
endmacro()

# Wrap JSON model files and assets into automated .hxx headers
wrapped_dependency(TEXT   src/model/data/world_node.json     WORLD_NODES_JSON)
wrapped_dependency(TEXT   src/model/data/world_path.json     WORLD_PATHS_JSON)
wrapped_dependency(TEXT   src/model/data/world_region.json   WORLD_REGIONS_JSON)
wrapped_dependency(TEXT   src/game/data/item.json            ITEMS_JSON)
wrapped_dependency(TEXT   src/model/data/item_source.json    ITEM_SOURCES_JSON)
wrapped_dependency(RAW    src/assets/patched_balance.bin     PATCHED_BALANCE)

set(SOURCES
        # --- Auto-generated dependencies ----------------------------------------
        "src/game/data/item.json.hxx"
        "src/model/data/world_node.json.hxx"
        "src/model/data/world_path.json.hxx"
        "src/model/data/item_source.json.hxx"
        "src/model/data/world_region.json.hxx"
        "src/assets/patched_balance.bin.hxx"

        # --- Constnats ----------------------------------------
        "src/constants/item_codes.hpp"
        "src/constants/item_names.hpp"
        "src/constants/item_names.cpp"
        "src/constants/flags.hpp"

        # --- I/O ----------------------------------------
        "src/io/graphviz_writer.cpp"
        "src/io/io.hpp"
        "src/io/model_writer.cpp"
        "src/io/spoiler_writer.cpp"

        # --- Game model ----------------------------------------
        "src/game/item.hpp"
        "src/game/item.cpp"
        "src/game/game_data.hpp"
        "src/game/game_data.cpp"

        # --- Logic model ----------------------------------------
        "src/model/item_source.hpp"
        "src/model/item_source.cpp"
        "src/model/world_node.hpp"
        "src/model/world_node.cpp"
        "src/model/world_path.hpp"
        "src/model/world_path.cpp"
        "src/model/world_region.hpp"
        "src/model/world_region.cpp"
        "src/model/randomizer_world.hpp"
        "src/model/randomizer_world.cpp"

        # --- Patches ----------------------------------------
        "src/patches/patch_new_game.hpp"
        "src/patches/patch_fix_map_inconsistencies.hpp"
        "src/patches/patch_original_game_balance.hpp"
        "src/patches/patch_neutralize_map_variant_changes.hpp"
        "src/patches/patch_change_flag_checks.hpp"
        "src/patches/patch_allow_using_consumables_while_full.hpp"

        "src/patches/game_patch.hpp"
        "src/patches/patches.hpp"
        "src/patches/patches.cpp"

        # --- Tools ----------------------------------------
        "src/tools/argument_dictionary.hpp"
        "src/tools/base64.hpp"
        "src/tools/base64.cpp"
        "src/tools/binary_file.hpp"
        "src/tools/binary_file.cpp"
        "src/tools/bitstream_reader.hpp"
        "src/tools/bitstream_reader.cpp"
        "src/tools/bitstream_writer.hpp"
        "src/tools/byte_array.hpp"
        "src/tools/byte_array.cpp"
        "src/tools/exception.hpp"
        "src/tools/json.hpp"
        "src/tools/mips_tools.hpp"
        "src/tools/mips_tools.cpp"
        "src/tools/stringtools.hpp"
        "src/tools/vectools.hpp"
        "src/tools/json_tools.hpp"
        "src/tools/psx_exe_file.hpp"

        # --- Project root ----------------------------------------
        "src/personal_settings.hpp"
        "src/personal_settings.cpp"
        "src/randomizer_options.hpp"
        "src/randomizer_options.cpp"
        "src/world_shuffler.hpp"
        "src/world_shuffler.cpp"
        "src/world_solver.hpp"
        "src/world_solver.cpp"
        "src/main.cpp"
        )

add_executable(alundra-randomizer "${SOURCES}")
