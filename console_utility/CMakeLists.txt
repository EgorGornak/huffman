cmake_minimum_required(VERSION 3.05)
project(huffman)

set(CMAKE_CXX_STANDARD 11)

add_executable(huffman ../library/Huffman.cpp ../library/Huffman.h ../console_utility/main.cpp)