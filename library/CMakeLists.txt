cmake_minimum_required(VERSION 3.05)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_LIB Huffman.cpp)

add_library(Huffman STATIC ${SOURCE_LIB})