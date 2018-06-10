//
// Created by egor on 09.06.18.
//

#include "Huffman.h"
#include <fstream>
#include <memory>
#include <vector>
#include <iostream>
#include <algorithm>
#include <set>

void Huffman::encode(std::istream &input, std::ostream &output) {
    if (input.peek() == EOF) {
        return;
    }

    std::vector <int> count(256);
    while(input.peek() != EOF) {
        char c;
        input.read(&c, sizeof(char));

        count[c + 128]++;
    }


    Node *root = build_tree(count);
    std::vector<bool> encoded_letter[256];
    calc_encoded_letter(root, encoded_letter, std::vector<bool>(0));

    std::vector<bool> round_tree;
    go_round(root, round_tree);
    round_tree.pop_back();

    uint16_t size_tree = round_tree.size();
    output.write((char*)&size_tree, sizeof(uint16_t)); // tree_size
    push_buffer(output, round_tree, true);                  // tree

    std::vector<char> round_letters;
    go_round(root, round_letters);
    uint16_t size_letters = round_letters.size();        // letters_size
    output.write((char*)&size_letters, sizeof(uint16_t)); // letters
    for (char c: round_letters) {
        output.write(&c, sizeof(char));
    }

    uint8_t len =  (8 - get_len(root, count, 0) % 8) % 8;  // count of ignore bites
    output.write((char*)&len, sizeof(uint8_t));

    input.seekg(std::istream::beg);
    std::vector<bool> buffer;
    while(input.peek() != EOF) {
        char c;
        input.read(&c, sizeof(char));
        buffer.insert(buffer.end(), encoded_letter[c + 128].begin(), encoded_letter[c + 128].end());
        push_buffer(output, buffer, false);
    }
    push_buffer(output, buffer, true);
    delete root;

}

void Huffman::decode(std::istream &input, std::ostream &output) {
    if (input.peek() == EOF) {
        return;
    }
    if (!check_hash(input)) {
        throw;
    }

    uint16_t size_tree;   //  read tree
    input.read((char*)&size_tree, sizeof(uint16_t));
    std::vector<bool> round_tree;
    for (int i = 0; i < (size_tree + 7)/ 8; i++) {
        uint8_t round;
        input.read((char*)&round, sizeof(uint8_t));
        for (int j = 0; j < 8; j++) {
            round_tree.push_back(bool(round & (1 << 7)));
            round <<= 1;
        }
    }
    while(round_tree.size() != size_tree) {
        round_tree.pop_back();
    }

    uint16_t count_letters;
    input.read((char*)&count_letters, sizeof(uint16_t));
    std::vector<char> letters(count_letters);
    for (int i = 0; i < count_letters; i++) {
        input.read(&letters[i], sizeof(char));
    }
    Node* root = new Node();
    int pos_round = 0;
    int pos_letters = 0;
    build_tree(root, round_tree, pos_round, letters, pos_letters);
    uint8_t ignore_rest;
    input.read((char*)&ignore_rest, sizeof(uint8_t));

    Node *curr = root;
    while(input.peek() != EOF) {
        uint8_t byte;
        input.read((char*)&byte, sizeof(uint8_t));

        int ignore = 0;
        if (input.peek() == EOF) {
            ignore = ignore_rest;
        }
        for (int j = 0; j < 8 - ignore; j++) {
            bool x = bool(byte & (1 << 7));
            if (x == 0) {
                curr = curr -> left;
            } else {
                curr = curr -> right;
            }
            if (curr -> left == nullptr) {
                output.write(&(curr -> c), sizeof(char));
                curr = root;
            }
            byte <<= 1;
        }
    }
    delete root;
}

Huffman::Node* Huffman::build_tree(std::vector<int> &count) {
    std::vector<std::pair<int, char> > weight;
    for (int i = 0; i < count.size(); i++) {
        if (count[i] != 0) {
            weight.push_back(std::make_pair(count[i], i - 128));
        }
    }
    std::sort(weight.begin(), weight.end());

    if (weight.size() == 1) {
        Node *list = new Node(weight[0].second, weight[0].first);
        Node *root = new Node();
        root -> left = list;
        return root;
    }


    std::multiset<Node*, Comp_node> s;

    for (auto x: weight) {
        s.insert(new Node(x.second, x.first));
    }

    while(s.size() > 1) {
        Node* a = *s.begin();
        s.erase(s.begin());
        Node *b = *s.begin();
        s.erase(s.begin());
        Node *tmp = new Node();
        tmp -> weight = a -> weight + b -> weight;
        tmp -> left = a;
        tmp -> right = b;
        s.insert(tmp);
    }

    return *s.begin();
}

bool Huffman::check_hash(std::istream &input) {
    return true;
}

void Huffman::calc_encoded_letter(Huffman::Node *vertex, std::vector<bool> encoded_letters[], std::vector<bool> curr) {
    if (vertex -> left == nullptr && vertex -> right == nullptr) {
        encoded_letters[vertex->c + 128] = curr;
        return;
    }
    if (vertex -> left != nullptr) {
        curr.push_back(0);
        calc_encoded_letter(vertex->left, encoded_letters, curr);
        curr.pop_back();
    }
    if (vertex -> right != nullptr) {
        curr.push_back(1);
        calc_encoded_letter(vertex->right, encoded_letters, curr);
        curr.pop_back();
    }
}

void Huffman::push_buffer(std::ostream &output, std::vector<bool> &v, bool flag) {
    for (int i = 0; i < (v.size() / 8) * 8; i += 8) {
        unsigned char tmp = 0;
        for (int j = i; j < i + 8; j++) {
            tmp <<= 1;
            tmp += v[j];
        }
        output.write((char*)&tmp, sizeof(char));
    }
    if (flag) {
        if (v.size() % 8 != 0) {
            unsigned char tmp = 0;
            for (int i = (v.size() / 8) * 8; i < v.size(); i++) {
                tmp <<= 1;
                tmp += v[i];
            }
            tmp <<= (8 - v.size() % 8);
            output.write((char*)&tmp, sizeof(char));
        }
    } else {
        std::vector <bool> tmp;
        for (int i = (v.size() / 8) * 8; i < v.size(); i++) {
            tmp.push_back(v[i]);
        }
        v = tmp;
    }

}

void Huffman::go_round(Huffman::Node *vertex, std::vector<bool> &curr) {
    if (vertex -> left != nullptr) {
        curr.push_back(0);
        go_round(vertex -> left, curr);
    }
    if (vertex -> right != nullptr) {
        curr.push_back(0);
        go_round(vertex -> right, curr);
    }
    curr.push_back(1);
}

void Huffman::go_round(Huffman::Node *vertex, std::vector<char> &curr) {
    if (vertex -> left == nullptr && vertex -> right == nullptr) {
        curr.push_back(vertex -> c);
    }
    if (vertex -> left != nullptr) {
        go_round(vertex->left, curr);
    }
    if (vertex -> right != nullptr) {
        go_round(vertex->right, curr);
    }
}

size_t Huffman::get_len(Huffman::Node *vertex, std::vector<int> &count, int deep) {
    if (vertex -> left == nullptr && vertex -> right == nullptr) {
        return count[vertex -> c + 128] * deep;
    }
    size_t summ = 0;
    if (vertex -> left != nullptr) {
        summ += get_len(vertex -> left, count, deep + 1);
    }
    if (vertex -> right != nullptr) {
        summ += get_len(vertex -> right, count, deep + 1);
    }
    return summ;
}

void Huffman::build_tree(Huffman::Node *&vertex, std::vector<bool> &round, int &pos_round, std::vector<char> &letters, int &pos_letters) {
    if (round[pos_round] == 1) {
        if (vertex -> left == nullptr && vertex -> right == nullptr) {
            vertex -> c = letters[pos_letters++];
        }
    } else {
        vertex -> left = new Node();
        build_tree(vertex -> left, round, ++pos_round, letters, pos_letters);
        vertex -> right = new Node();
        build_tree(vertex -> right, round, ++pos_round, letters, pos_letters);
    }
    pos_round++;
}
