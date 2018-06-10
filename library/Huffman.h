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
        Node *left, *right;
        ~Node() {
            if (left != nullptr) {
                delete left;
            }
            if (right != nullptr) {
                delete right;
            }
        };
        Node() {};
        Node(char c, int weight) : c(c), weight(weight), left(nullptr), right(nullptr) {};

        bool operator<(const Node& other) const
        {
            return weight < other.weight;
        }
    };
    struct Comp_node
    {
        bool operator()(Node* x, Node* y) const {
            return x->weight < y->weight;
        }
    };

    static size_t get_len(Node* vertex, std::vector <int> &count, int deep);
    static void go_round(Node* vertex, std::vector <bool> &curr);
    static void go_round(Node* vertex, std::vector <char> &curr);
    static void push_buffer(std::ostream &output, std::vector <bool> &v, bool flag);
    static bool check_hash(std::istream &input);
    static Node* build_tree(std::vector<int> &count);
    static void build_tree(Node* &vertex, std::vector<bool> &round, int &pos_round, std::vector<char> &letters, int &pos_letters);
    static void calc_encoded_letter(Node* vertex, std::vector<bool> encoded_letters[], std::vector<bool> curr);
};


#endif //HUFFMAN_HUFFMAN_H
