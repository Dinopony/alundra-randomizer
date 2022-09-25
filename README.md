![version](https://img.shields.io/badge/Version-1.0.0-blue)
![compile](https://github.com/Dinopony/alundra-randomizer/workflows/Compile/badge.svg)

# Alundra Randomizer

This project is a randomizer for the excellent action RPG Alundra, released on the first PlayStation in 1997.
It works on a 1.1 US image of the game by randomizing item sources and altering the game to make it more enjoyable in a randomizer format.

## Game flow

- Game starts inside Jess's house
- Most cutscenes and scenario-related blockers are removed to put the focus on gameplay & exploration
- The global goal is to find the 7 crests, open Lake Shrine using those and beat Melzas
- All arbitrary conditions from the original game tend to stay the same whenever relevant (e.g. Secret Pass to open the Casino)
- Keys (including Elevator Key) are not shuffled to keep the dungeon flow similar to the vanilla game

## Usage

### Release versions

Using the provided release packages (on the right panel of this webpage) is really straightforward since everything
(excepted the original ROM, of course) is included for you to start randomizing.

You just have to put your own copy of the original 1.1 US game disc inside the folder named `input.bin`, and use the 
`gen_preset.bat` script on Windows (Linux users are expected to know how to use CLI).

A command line interface will open, don't be scared! It will ask you for which **preset** you want to use. Presets are a
bunch of settings that define how the game will be randomized, and there is a default one provided with the randomizer.
These can be found under the `presets/` subfolder, and you can duplicate the existing ones and modify them to choose
your own settings if the default ones are not your cup of tea.
Back to the command line, you can just press Enter to use the default preset or you can type your own preset name before
confirming.

The randomizer will start working and generate a seed for you inside the `seeds/` subfolder, so you can play on your
favorite emulator / real console. It will also give you a **permalink**, which is a sort of code you can give your
randomizer pals so they can generate the exact same seed as you by using `gen_permalink.bat` and pasting this permalink.

Have fun randomizing!

### Building from source

If you want to get the very last version, you can also compile and use the executable version on your own device. You
will need CMake to do so, and then do:

- On Windows, with Visual Studio 2019 installed, you can execute `build_win_vs2019.bat`
- On Unix, you can execute `build_unix.sh`
