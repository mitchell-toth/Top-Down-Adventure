cmake_minimum_required(VERSION 3.17)
project(Game2)

set(CMAKE_CXX_STANDARD 14)
set(SFML_DIR ../SFML/lib/cmake/SFML)    # Location of SFML (local to project)
set(ENV{PATH} mingw32/bin/)             # Location of C++ compiler

# Include locations
include_directories(. ../SFML/include ../)

# Include SFML
find_package(SFML COMPONENTS audio network graphics window system REQUIRED)

# Add the Engine library
add_library(TDAHelper SHARED ../TDAHelper/library.cpp)
target_link_libraries(TDAHelper sfml-audio sfml-network sfml-graphics sfml-window sfml-system)

add_executable(Game2 main.cpp Game.cpp Game.h Maps.cpp Maps.h)
target_link_libraries(Game2 TDAHelper)