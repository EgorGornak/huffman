cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_LIB Huffman.cpp)

add_library(Huffman STATIC ${SOURCE_LIB})