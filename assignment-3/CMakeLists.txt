cmake_minimum_required(VERSION 3.10)

project(assignment-2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "-fsanitize=address")

find_package(Catch2 REQUIRED)


# unit tests with Catch2

add_executable(unit_test queue.h arrayqueue.h linkedqueue.h tests.cpp)
target_link_libraries(unit_test PRIVATE Catch2::Catch2WithMain)

include(CTest)
include(Catch)

catch_discover_tests(unit_test)

add_executable(console_test 
  console_test.cpp queue.h arrayqueue.h linkedqueue.h console_helper.h console_helper.cpp)

add_executable(bank
  bank.cpp queue.h arrayqueue.h linkedqueue.h console_helper.h console_helper.cpp)

