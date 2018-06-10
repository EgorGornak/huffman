//
// Created by egor on 10.06.18.
//

#include <cassert>
#include <iostream>
#include <fstream>
#include "gtest.h"
#include "../library/Huffman.h"

bool compareFiles(const std::string &p1, const std::string& p2) {
    std::ifstream f1(p1, std::ifstream::binary);
    std::ifstream f2(p2, std::ifstream::binary);

    if (f1.fail() || f2.fail()) {
        return false;
    }

    if (f1.tellg() != f2.tellg()) {
        return false;
    }

    f1.seekg(0, std::ifstream::beg);
    f2.seekg(0, std::ifstream::beg);
    return std::equal(std::istreambuf_iterator<char>(f1.rdbuf()),
                      std::istreambuf_iterator<char>(),
                      std::istreambuf_iterator<char>(f2.rdbuf()));
}

void encode(const std::string &input_file, const std::string &output_file) {
    std::ifstream input(input_file, std::ifstream::binary);
    std::ofstream output(output_file, std::ofstream::binary);
    Huffman::encode(input, output);
    input.close();
}
void decode(const std::string &input_file, const std::string &output_file) {
    std::ifstream input(input_file, std::ifstream::binary);
    std::ofstream output(output_file, std::ofstream::binary);

    Huffman::decode(input, output);
}

bool encode_decode_file(const std::string &input_file) {
    encode(input_file, "encoded_file");
    decode("encoded_file", "decoded_file");

    return compareFiles(input_file, "decoded_file");

}

TEST(correctness, eight_letters_b)
{
    std::stringstream input("bbbbbbbb"); // b count multiplicity 8
    std::stringstream encode;
    std::stringstream decode;
    Huffman::encode(input, encode);
    Huffman::decode(encode, decode);

    EXPECT_EQ(input.str(), decode.str());
}


TEST(correctness, letters_a)
{
    std::stringstream input("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    std::stringstream encode;
    std::stringstream decode;
    Huffman::encode(input, encode);
    Huffman::decode(encode, decode);

    EXPECT_EQ(input.str(), decode.str());
}

TEST(correctness, spoiled_file)
{
    std::stringstream input("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    std::stringstream encode;
    std::stringstream decode;
    Huffman::encode(input, encode);
    char c = 'a';
    encode.write(&c, sizeof(char));
    EXPECT_ANY_THROW(Huffman::decode(encode, decode));
}

TEST(correctness, empty_file)
{
    EXPECT_TRUE(encode_decode_file("../tests/empty"));
}

TEST(correctness, mp3)
{
   EXPECT_TRUE(encode_decode_file("../tests/music.mp3"));
}

TEST(correctness, video)
{
    EXPECT_TRUE(encode_decode_file("../tests/video.mp4"));
}

TEST(correctness, pdf)
{
    EXPECT_TRUE(encode_decode_file("../tests/EffectiveCPlusPlus.pdf"));
}

TEST(correctness, jpg)
{
    EXPECT_TRUE(encode_decode_file("../tests/white.jpg"));
}

TEST(correctness, random_string) {
    unsigned int count_of_strings = 1000;
    for (unsigned int i = 0; i < count_of_strings; i++) {
        std::string tmp;
        for (unsigned int j = 0; j < 5000; j++) {
            tmp += char(rand() % 256 - 128);
        }
        std::stringstream input(tmp);
        std::stringstream encode;
        std::stringstream decode;
        Huffman::encode(input, encode);
        Huffman::decode(encode, decode);

        EXPECT_EQ(input.str(), decode.str());
    }
}