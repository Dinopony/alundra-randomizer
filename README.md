![version](https://img.shields.io/badge/Version-0.5.0-blue)
![compile](https://github.com/Dinopony/alundra-randomizer/workflows/Compile/badge.svg)

# Alundra Randomizer

This project is a randomizer for the excellent action RPG Alundra, released on the first PlayStation in 1997.
It works on a 1.1 US image of the game by randomizing item sources and altering the game to make it more enjoyable in a randomizer format.

## Notable differences compared to original game

### Game flow

- Game starts inside Jess's house
- Most cutscenes and scenario-related blockers are removed to put the focus on gameplay & exploration
- The global goal is to find the 7 crests, open Lake Shrine using those and beat Melzas
- All arbitrary conditions from the original game tend to stay the same (e.g. Secret Pass to open the Casino)
- Keys (including Elevator Key) are not shuffled to keep the dungeon flow similar to the vanilla game

## Usage

### Release versions

Using the provided release packages (on the right panel of this webpage) is really straightforward since everything
(excepted the original ROM, of course) is included for you to start randomizing.

You just have to put your own copy of the original 1.1 US game disc inside the folder named `input.bin`, and use the 
`gen_preset.bat` script on Windows (Linux users are expected to know how to use CLI).

### Building from source

If you want to get the very last version, you can also compile and use the executable version on your own device. You
will need CMake to do so, and then do:

- On Windows, with Visual Studio 2019 installed, you can execute `build_win_vs2019.bat`
- On Unix, you can execute `build_unix.sh`
