cmake_minimum_required(VERSION 3.10)
project(huffman)

set(CMAKE_CXX_STANDARD 17)

add_executable(huffman library/Huffman.cpp library/Huffman.h console_utility/main.cpp tests/huffman_testing.cpp)

add_executable(huffman_testing
        tests/huffman_testing.cpp
        library/Huffman.cpp
        library/Huffman.h
        tests/gtest-all.cc
        tests/gtest.h
        tests/gtest_main.cc)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -pedantic")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined -D_GLIBCXX_DEBUG")
endif()


target_link_libraries(huffman_testing -lgmp -lgmpxx -lpthread)