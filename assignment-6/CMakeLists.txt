cmake_minimum_required(VERSION 3.10)

project(assignment-6)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")

add_executable(
  main
  main.cpp 
  utils.h
  utils.cpp
)

add_executable(
  generator
  generator.cpp 
  utils.h
  utils.cpp
)

add_executable(
  checker
  checker.cpp 
  utils.h
  utils.cpp
)
