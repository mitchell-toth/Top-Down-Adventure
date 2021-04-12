cmake_minimum_required(VERSION 3.17)
project(Game2)

set(CMAKE_CXX_STANDARD 14)
set(SFML_DIR ../SFML/lib/cmake/SFML)    # Location of SFML (local to project)
set(ENV{PATH} mingw32/bin/)             # Location of C++ compiler

# Include SFML
include_directories(. SFML/include)
find_package(SFML COMPONENTS graphics window audio system network REQUIRED)

# Add the Engine library
add_library(TDAHelper ../TDAHelper/library.hpp ../TDAHelper/library.cpp)

add_executable(Game2 main.cpp)
target_link_libraries(Game2 sfml-graphics sfml-window sfml-audio sfml-system sfml-network -static-libstdc++ TDAHelper)