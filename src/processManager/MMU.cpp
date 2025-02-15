#include "../includes/MMU.hpp"

int MMU::traduzirEndereco(int enderecoLogico)
{
    const int OFFSET = 1000; // Simula deslocamento na memória virtual
    return enderecoLogico + OFFSET;
}

int MMU::obterIndiceTabela(const std::vector<int> &tabelaBinaria)
{
    int indice = 0;
    for (size_t i = 0; i < tabelaBinaria.size(); ++i)
    {
        indice = (indice << 1) | tabelaBinaria[i]; // Converte binário para decimal
    }
    return indice;
}
