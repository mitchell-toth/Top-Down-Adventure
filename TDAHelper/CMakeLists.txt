cmake_minimum_required(VERSION 3.17)
project(TDAHelper)

set(CMAKE_CXX_STANDARD 14)
set(SFML_DIR ../SFML/lib/cmake/SFML)    # Location of SFML (local to project)
set(ENV{PATH} mingw32/bin/)             # Location of C++ compiler

include_directories(. ../SFML/include)
find_package(SFML COMPONENTS audio network graphics window system REQUIRED)

add_library(TDAHelper SHARED library.hpp library.cpp)
target_link_libraries(TDAHelper sfml-audio sfml-network sfml-graphics sfml-window sfml-system -static-libstdc++)