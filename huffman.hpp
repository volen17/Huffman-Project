#include <string>
#include <vector>
#include <unordered_map>
#pragma once

struct BTree
{
    int frequency = 0;
    char symbol = '\0';
    BTree *left = nullptr;
    BTree *right = nullptr;
    BTree() = default;
    BTree(const BTree &);
    BTree &operator=(const BTree &);
    ~BTree();
};

class Huffman
{
public:
    Huffman() = default;
    Huffman(std::string);
    Huffman(std::string, std::vector<std::pair<char, int>>);
    Huffman(const Huffman &);
    Huffman &operator=(const Huffman &);
    ~Huffman();

    void pushToFrequencyTable(std::pair<char, int>);

    BTree *get_tree() const;
    std::string get_text() const;
    std::string get_compressedText() const;
    std::string get_decompressedText() const;
    std::vector<std::pair<char, int>> get_frequencyTable() const;
    std::unordered_map<char, std::string> get_codes() const;

    friend std::ostream &operator<<(std::ostream &, const Huffman &);

    bool isInTable(char);
    void makeFrequencyTable();
    void buildHuffmanTree();
    void makeCodingTable(BTree *, std::unordered_map<char, std::string> &, std::string);

    void printTree(BTree *);
    void printCodes();

    void compress();
    void decompressHelper(BTree *, std::string, std::string &, int &);
    void decompress();

    std::string debug();
    double level();

private:
    short binaryToDecimal(std::string);
    BTree *tree = nullptr;
    std::string text = "";
    std::string compressedText = "";
    std::string decompressedText = "";
    std::vector<std::pair<char, int>> frequencyTable{};
    std::unordered_map<char, std::string> codes{};
};