#ifndef MMU_HPP
#define MMU_HPP

#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>

using namespace std;

class MMU
{
public:
    static int traduzirEndereco(int enderecoLogico);
    static int obterIndiceTabela(const std::vector<int> &tabelaBinaria);
};

#endif
