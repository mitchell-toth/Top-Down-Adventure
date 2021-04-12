# Top-Down-Adventure
A basic game engine built in C++ and SFML designed especially for top-down adventure games. Provides support for sprite sheets and easy map generation. Also included in this repo are example games that demo the engine's features.

# Build
This project is set up such that the "TDAHelper" game engine is a separate build from each demo game, and each demo game is a separate build itself. This makes for 3 separate builds that are best run in their own C++ CMake projects.

For example, using an IDE like CLion or Visual Studio:

- Before opening the IDE, create an empty directory for each of the builds you want to run.
- In the IDE, create a new C++ CMake build for each of the empty directories you created, using each as the corresponding build path. Create a Shared Library build for the engine and a standard Executable build for the game demos.
- Pull the code from this repo and copy over the contents to each corresponding build. Ensure that a local copy of SFML is a top-level sibling to the builds.
- If necessary, modify each build's CMakeLists.txt to work with your machine's setup. You may need to change the path specified for the C++ compiler.

# Include

This setup has the advantage of modularizing the game engine ("TDAHelper") and making it distinct from each demo game. A game developer desiring to use TDAHelper needs only to `#include` the library after completing the following steps:

- Specify the path where TDAHelper is located on your machine. Do this withing the `include_directories` directive within each game's CMakeLists.txt.
- Specify the locations of TDAHelper/library.hpp and TDAHelper/library.cpp within the `add_library` directive of each game's CMakeLists.txt.
- Then simply write the line `#include <TDAHelper/library.hpp>` where needed in your game.

Constants and methods from TDAHelper will be scoped by the 'td' namespace.

# Run

Run each game using the IDE's run action.
