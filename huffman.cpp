#include "huffman.hpp"
#include <iostream>
#include <stack>
#include <algorithm>

BTree::BTree(const BTree &other)
{
    frequency = other.frequency;
    symbol = other.symbol;
    left = new BTree;
    *left = *other.left;
    right = new BTree;
    *right = *other.right;
}

BTree &BTree::operator=(const BTree &other)
{
    if (this != &other)
    {
        frequency = other.frequency;
        symbol = other.symbol;
        left = new BTree;
        *left = *other.left;
        right = new BTree;
        *right = *other.right;
    }
    return *this;
}

BTree::~BTree()
{
    if (this->left != nullptr)
    {
        delete this->left;
    }
    if (this->right != nullptr)
    {
        delete this->right;
    }
}

Huffman::Huffman(std::string text)
{
    this->text = text;
    decompressedText = text;
    makeFrequencyTable();
    buildHuffmanTree();
    makeCodingTable(tree, codes, "");
    compress();
}

Huffman::Huffman(std::string compressedText, std::vector<std::pair<char, int>> frequencyTable)
{
    this->compressedText = compressedText;
    this->frequencyTable = frequencyTable;
    buildHuffmanTree();
    makeCodingTable(tree, codes, "");
    decompress();
}

Huffman::Huffman(const Huffman &other)
{

    text = other.text;
    compressedText = other.compressedText;
    decompressedText = other.decompressedText;
    frequencyTable = other.frequencyTable;
    codes = other.codes;
}

Huffman &Huffman::operator=(const Huffman &other)
{
    if (this != &other)
    {
        text = other.text;
        compressedText = other.compressedText;
        decompressedText = other.decompressedText;
        frequencyTable = other.frequencyTable;
        codes = other.codes;
    }
    return *this;
}

Huffman::~Huffman()
{
    delete tree;
}

void Huffman::pushToFrequencyTable(std::pair<char, int> p)
{
    frequencyTable.push_back(p);
}

BTree *Huffman::get_tree() const
{
    return tree;
}

std::string Huffman::get_text() const
{
    return text;
}

std::string Huffman::get_compressedText() const
{
    return compressedText;
}

std::string Huffman::get_decompressedText() const
{
    return decompressedText;
}

std::vector<std::pair<char, int>> Huffman::get_frequencyTable() const
{
    return frequencyTable;
}

std::unordered_map<char, std::string> Huffman::get_codes() const
{
    return codes;
}

bool Huffman::isInTable(char c)
{
    for (int i = 0; i < frequencyTable.size(); i++)
    {
        if (c == frequencyTable[i].first)
        {
            return true;
        }
    }
    return false;
}

void Huffman::makeFrequencyTable()
{
    char temp;
    int frequency;
    for (int i = 0; i < text.length(); i++)
    {
        if (isInTable(text[i]))
        {
            continue;
        }
        temp = text[i];
        frequency = 0;
        for (int j = 0; j < text.length(); j++)
        {
            if (temp == text[j])
            {
                frequency++;
            }
        }
        frequencyTable.push_back(std::make_pair(temp, frequency));
    }
    std::sort(frequencyTable.begin(), frequencyTable.end(), [](auto p1, auto p2) {
        return p1.second > p2.second;
    });
}

void Huffman::buildHuffmanTree()
{
    std::stack<BTree *> trees;
    for (int i = 0; i < frequencyTable.size(); i++)
    {
        BTree *root = new BTree();
        root->symbol = frequencyTable[i].first;
        root->frequency = frequencyTable[i].second;
        trees.push(root);
    }
    while (trees.size() > 1)
    {
        BTree *tempLeft = new BTree();
        BTree *tempRight = new BTree();
        BTree *tempRoot = new BTree();
        if (trees.size() % 2 == 0)
        {
            tempRight = trees.top();
            trees.pop();
            tempLeft = trees.top();
            trees.pop();
            tempRoot->frequency = tempLeft->frequency + tempRight->frequency;
            tempRoot->left = tempLeft;
            tempRoot->right = tempRight;
        }
        else
        {
            tempLeft = trees.top();
            trees.pop();
            tempRight = trees.top();
            trees.pop();
            tempRoot->frequency = tempLeft->frequency + tempRight->frequency;
            tempRoot->left = tempLeft;
            tempRoot->right = tempRight;
        }
        trees.push(tempRoot);
    }
    tree = trees.top();
}

void Huffman::makeCodingTable(BTree *tree, std::unordered_map<char, std::string> &codes, std::string code)
{
    if (tree->left == nullptr && tree->right == nullptr)
    {
        codes[tree->symbol] = code;
    }
    if (tree->left != nullptr)
    {
        makeCodingTable(tree->left, codes, code + "0");
    }
    if (tree->right != nullptr)
    {
        makeCodingTable(tree->right, codes, code + "1");
    }
}

void Huffman::printCodes()
{
    for (auto const &pair : codes)
    {
        std::cout << "{" << pair.first << ": " << pair.second << "}\n";
    }
}

void Huffman::compress()
{
    compressedText.clear();
    for (char x : text)
    {
        compressedText += codes[x];
    }
}

void Huffman::decompressHelper(BTree *tree, std::string compressedText, std::string &decompressedText, int &cnt)
{
    if (tree->left == nullptr && tree->right == nullptr)
    {
        decompressedText += tree->symbol;
    }
    if (compressedText.front() == '1' && tree->left != nullptr)
    {
        cnt++;
        decompressHelper(tree->right, compressedText.substr(1), decompressedText, cnt);
    }
    if (compressedText.front() == '0' && tree->right != nullptr)
    {
        cnt++;
        decompressHelper(tree->left, compressedText.substr(1), decompressedText, cnt);
    }
}

void Huffman::decompress()
{
    int cnt = 0;
    while (cnt != compressedText.length())
    {
        decompressHelper(tree, compressedText.substr(cnt), decompressedText, cnt);
    }
}

short Huffman::binaryToDecimal(std::string n)
{
    short result = 0;
    for (int i = 0; i < n.size(); i++)
    {
        result = result * 2 + n[i] - '0';
    }
    return result;
}

std::string Huffman::debug()
{
    std::string debugString = "";
    int i = 0;
    do
    {
        debugString += (std::to_string(binaryToDecimal(compressedText.substr(i * 8, 8))) + " ");
        i++;
    } while (compressedText.substr(i * 8).length() > 8);
    debugString += std::to_string(binaryToDecimal(compressedText.substr(i * 8)));
    return debugString;
}

double Huffman::level()
{
    return (double)((decompressedText.length() * 8) / compressedText.length());
}

void Huffman::printTree(BTree *tree)
{
    std::cout << tree->symbol << " " << tree->frequency << std::endl;
    if (tree->left != nullptr)
    {
        printTree(tree->left);
    }
    if (tree->right != nullptr)
    {
        printTree(tree->right);
    }
}

std::ostream &operator<<(std::ostream &out, const Huffman &tree)
{
    out << tree.compressedText << std::endl
        << tree.frequencyTable.size() << std::endl;
    for (int i = 0; i < tree.frequencyTable.size(); i++)
    {
        if (tree.frequencyTable[i].first == '\n')
        {
            out << "\\n";
        }
        else
        {
            out << tree.frequencyTable[i].first;
        }
        out << "-" << tree.frequencyTable[i].second << std::endl;
    }
    out << std::endl;
    return out;
}