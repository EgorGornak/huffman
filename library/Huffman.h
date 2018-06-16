#ifndef HUFFMAN_HUFFMAN_H
#define HUFFMAN_HUFFMAN_H

#include <iosfwd>
#include <memory>
#include <vector>
#include <iostream>

class Huffman {
public:
    static void encode(std::istream &input, std::ostream &output);
    static void decode(std::istream &input, std::ostream &output);
private:
    struct Node{
        char c;
        int weight;
        std::shared_ptr<Node> left, right;
        Node() : left(nullptr), right(nullptr) {};
        Node(char c, int weight) : c(c), weight(weight), left(nullptr), right(nullptr) {};

        bool operator<(const Node& other) const
        {
            return weight < other.weight;
        }
    };
    struct Comp_node {
        bool operator()(const std::shared_ptr<Node> &x, const std::shared_ptr<Node> &y) const {
            return x->weight < y->weight;
        }
    };


    static uint32_t get_len(const std::shared_ptr<Node> &vertex, std::vector <int> &count, uint32_t deep);
    static void go_round(std::shared_ptr<Node> vertex, std::vector <bool> &curr);
    static void go_round(std::shared_ptr<Node> vertex, std::vector <char> &curr);
    static void push_buffer(std::ostream &output, std::vector <bool> &v, bool flag);
    static std::shared_ptr<Node> build_tree(std::vector<int> &count);
    static void build_tree(std::shared_ptr<Node> &vertex, std::vector<bool> &round, int &pos_round, std::vector<char> &letters, int &pos_letters);
    static void calc_encoded_letter(std::shared_ptr<Node> vertex, std::vector<bool> encoded_letters[], std::vector<bool> curr);
};


#endif //HUFFMAN_HUFFMAN_H
