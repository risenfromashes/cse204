cmake_minimum_required(VERSION 3.10)

project(assignment-3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "-fsanitize=address")

find_package(Catch2 REQUIRED)

add_executable(
  unit_test
  tests.cpp bstree.h
)
target_link_libraries(unit_test PRIVATE Catch2::Catch2WithMain)

include(CTest)
include(Catch)

catch_discover_tests(unit_test)

add_executable(
  main
  main.cpp bstree.h
)
