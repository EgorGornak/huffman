#include <iostream>
#include <fstream>
#include <vector>
#include "../library/Huffman.h"

int main (int argc, char ** argv) {
    if (argc != 4) {
        std::cout << "Usage: <input_file> <output_file> <-e|-d>\n";
        return 0;
    }
    char const * input_file = argv[1];
    char const * output_file = argv[2];
    std::string mode = argv[3];
    std::ifstream input(input_file, std::ifstream::binary);
    std::ofstream output(output_file, std::ofstream::binary);
    if (mode == "-e") {
        Huffman::encode(input, output);
    } else {
        try {
            Huffman::decode(input, output);
        } catch (std::exception e) {
            std::cerr << "spolied file :(\n";
        }
    }
    return 0;
}