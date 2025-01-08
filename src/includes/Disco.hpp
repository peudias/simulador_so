#ifndef DISCO_HPP
#define DISCO_HPP

#include "../includes/Registers.hpp"
#include "../includes/RAM.hpp"

#include <iostream>
#include <vector>
#include <mutex>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <filesystem>

using namespace std;

class Disco
{
private:
    static const int LINHAS = 10;
    static const int COLUNAS = 10;
    vector<vector<pair<int, bool>>> memoria;
    mutex mtx;

public:
    Disco();

    void write(int valor);
    void display(ofstream &outfile) const;
    void setRegistersFromFile(Registers &regs, const string &regsFilename);
    int loadInstructionsFromFile(RAM &ram, const string &instrFilename, int enderecoBase);
    vector<string> listInstructionsFile(const string &dir);
};

#endif
