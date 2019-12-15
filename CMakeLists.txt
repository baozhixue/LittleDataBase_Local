cmake_minimum_required(VERSION 3.15)
project(B_PC)

set(CMAKE_CXX_STANDARD 11)

add_executable(B_PC main.cpp BTree.h default.h List.h ListBTree.h ELEMENT.h ROW.h TEST.h Client.h DBCore.h Table.h)