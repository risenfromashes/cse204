cmake_minimum_required(VERSION 3.20)

project(assignment-9)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


add_executable(1 1.cpp)
add_executable(2 2.cpp)

add_executable(generator 1.h 2.h 1.cpp 2.cpp test_generator.cpp)
set_target_properties(generator PROPERTIES COMPILE_DEFINITIONS "TEST")
