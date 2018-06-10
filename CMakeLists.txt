cmake_minimum_required(VERSION 3.10)
project(huffman)

set(CMAKE_CXX_STANDARD 17)

add_executable(huffman library/Huffman.cpp library/Huffman.h console_utility/main.cpp)