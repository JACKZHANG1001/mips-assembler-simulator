cmake_minimum_required(VERSION 2.4)

project(mips-assembler-simulator)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)


add_library(assembler_lib STATIC
        ${PROJECT_SOURCE_DIR}/src/ass.cpp)
target_include_directories(assembler_lib
        PUBLIC
            ${PROJECT_SOURCE_DIR}/include/assembler)

add_library(simulator_lib STATIC
        ${PROJECT_SOURCE_DIR}/src/simu.cpp)
target_include_directories(simulator_lib
        PUBLIC
            ${PROJECT_SOURCE_DIR}/include/simulator)



add_executable(assembler src/assembler.cpp)
add_executable(simulator src/simulator.cpp)

target_link_libraries(assembler
            assembler_lib)

target_link_libraries(simulator_lib
            assembler_lib)

target_link_libraries(simulator
            simulator_lib)
