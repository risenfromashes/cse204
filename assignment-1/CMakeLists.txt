cmake_minimum_required(VERSION 3.10)

project(assignment-1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(test list.h arraylist.h linkedlist.h test.cpp)
add_executable(tnl list.h arraylist.h linkedlist.h tnl.cpp)
