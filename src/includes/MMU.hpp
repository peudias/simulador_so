#ifndef MMU_HPP
#define MMU_HPP

#include <unordered_map>
#include <string>
#include <bitset>
#include <fstream>

using namespace std;

class MMU
{
private:
    unordered_map<string, int> tabelaMMU; // Mapeia endereços virtuais (binário) para físicos (inteiros)

public:
    MMU();

    string gerarEnderecoVirtual(int id);
    int mapearParaFisico(const string &enderecoVirtual);
    void exibirTabelaMMU(ofstream &outfile);
};

#endif // MMU_HPP