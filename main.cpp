#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "huffman.hpp"

bool isTXT(std::string file)
{
    std::string extension;
    int i = file.length() - 1;
    while (file[i] != '.')
    {
        extension = extension + file[i];
        i--;
    }
    if (extension == "txt")
    {
        return true;
    }
    return false;
}

void printMenu()
{
    std::cout << "Huffman Compressor\n"
              << std::endl;
    std::cout << "- c[ompress]      -   Compress mode (default)\n"
              << "- d[ecompress]    -   Decompress mode\n"
              << "- i <filename>    -   Input file\n"
              << "- o <filename>    -   Output file\n"
              << "- e[xit]          -   exits the program\n"
              << "* de[bug]         -   Debug mode\n"
              << "* l[evel]         -   Level of compression\n"
              << std::endl;
}

void system()
{
    printMenu();
    Huffman h;
    bool mode = true; // compress (true) / decompress (false)
    bool isLoadedFile = false;
    bool isSavedFile = false;
    while (true)
    {
        std::string command;
        std::getline(std::cin, command);
        if (command.compare("c") == 0 || command.compare("compress") == 0)
        {
            mode = true;
            isSavedFile = false;
            isLoadedFile = false;
            std::cout << std::endl
                      << "Compress mode...\n"
                      << std::endl;
            continue;
        }
        if (command.compare("d") == 0 || command.compare("decompress") == 0)
        {
            isSavedFile = false;
            isLoadedFile = false;
            mode = false;
            std::cout << std::endl
                      << "Decompress mode...\n"
                      << std::endl;
            continue;
        }
        if (command.substr(0, 2).compare("i ") == 0 && command.length() > 2)
        {
            if (!isTXT(command.substr(2)))
            {
                std::cout << std::endl
                          << "This program works only with .txt files!\n"
                          << std::endl;
                continue;
            }
            if (mode) // compress mode
            {
                try
                {
                    std::string text;
                    std::ifstream in(command.substr(2));
                    if (in.good())
                    {
                        std::stringstream buffer;
                        buffer << in.rdbuf();
                        text = buffer.str();
                    }
                    in.close();
                    h = Huffman(text);
                }
                catch (...)
                {
                    std::cout << std::endl
                              << "Error reading file\n"
                              << std::endl;
                }
            }
            else // decompress mode
            {
                try
                {
                    std::string line;
                    std::string compressedText;
                    std::vector<std::pair<char, int>> frequencyTable;
                    int frequencyTableSize;
                    isLoadedFile = true;
                    isSavedFile = false;
                    std::ifstream in(command.substr(2));
                    if (in.good())
                    {
                        getline(in, line);
                        compressedText = line;
                        getline(in, line);
                        frequencyTableSize = std::stoi(line);
                        for (int i = 0; i < frequencyTableSize; i++)
                        {
                            char c;
                            std::string rest;
                            getline(in, line);
                            if (line.substr(0, 2).compare("\\n") == 0)
                            {
                                c = '\n';
                                rest = line.substr(3);
                            }
                            else
                            {
                                c = line[0];
                                rest = line.substr(2);
                            }
                            frequencyTable.push_back(std::make_pair(c, std::stoi(rest)));
                        }
                    }
                    h = Huffman(compressedText, frequencyTable);
                    in.close();
                }
                catch (...)
                {
                    std::cout << std::endl
                              << "Error reading compressed file\n"
                              << std::endl;
                }
            }
            isLoadedFile = true;
            isSavedFile = false;
            std::cout << std::endl;
            continue;
        }
        if (command.substr(0, 2).compare("o ") == 0 && command.length() > 2)
        {
            if (!isLoadedFile)
            {
                std::cout << std::endl
                          << "There is no loaded file!\n"
                          << std::endl;
                continue;
            }
            if (!isTXT(command.substr(2)))
            {
                std::cout << std::endl
                          << "This program works only with .txt files!\n"
                          << std::endl;
                continue;
            }
            std::ofstream out(command.substr(2));
            if (mode) // compress mode
            {
                out << h;
                out.close();
            }
            else // decompress mode
            {
                out << h.get_decompressedText();
                out.close();
            }
            isSavedFile = true;
            std::cout << std::endl;
            continue;
        }
        if (command.compare("exit") == 0)
        {
            if (!isSavedFile && isLoadedFile)
            {
                std::cout << std::endl
                          << "Your file isn't saved in an output file!\n";
                std::string yes_no;
                do
                {
                    std::cout << std::endl
                              << "Do you want to continue? (yes/no)\n"
                              << std::endl;
                    std::cin >> yes_no;
                } while (!(yes_no == "yes" || yes_no == "no"));
                if (yes_no == "no")
                {
                    continue;
                }
            }
            std::cout << std::endl
                      << "Exiting the program...";
            exit(0);
        }
        if (command.compare("de") == 0 || command.compare("debug") == 0)
        {
            if (!isLoadedFile)
            {
                std::cout << std::endl
                          << "There is no loaded file!\n"
                          << std::endl;
                continue;
            }
            std::cout << h.debug() << std::endl;
            continue;
        }
        if (command.compare("l") == 0 || command.compare("level") == 0)
        {
            if (!isLoadedFile)
            {
                std::cout << std::endl
                          << "There is no loaded file!\n"
                          << std::endl;
                continue;
            }
            std::cout << h.level() << std::endl;
            continue;
        }
        std::cout << std::endl
                  << "Wrong command!" << std::endl
                  << std::endl;
    }
}

int main()
{
    system();
    return 0;
}