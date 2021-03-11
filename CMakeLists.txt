cmake_minimum_required(VERSION 2.4)

project(mips-assembler-simulator)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${PROJECT_SOURCE_DIR})
add_executable(assembler assembler/src/ass.cpp)
