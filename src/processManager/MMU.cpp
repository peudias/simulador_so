#include "../includes/MMU.hpp"

MMU::MMU() {}

string MMU::gerarEnderecoVirtual(int id)
{
    bitset<8> enderecoBinario(id); // Converte o ID em um endereço binário de 8 bits
    string enderecoVirtual = enderecoBinario.to_string();

    // Mapeia o endereço virtual para um endereço físico sequencial
    if (tabelaMMU.find(enderecoVirtual) == tabelaMMU.end())
    {
        tabelaMMU[enderecoVirtual] = id; // O endereço físico será o próprio ID
    }

    return enderecoVirtual;
}

int MMU::mapearParaFisico(const string &enderecoVirtual)
{
    if (tabelaMMU.find(enderecoVirtual) != tabelaMMU.end())
    {
        return tabelaMMU[enderecoVirtual];
    }
    return -1; // Indica erro se o endereço virtual não estiver na tabela
}

void MMU::exibirTabelaMMU(ofstream &outfile)
{
    outfile << "\n================== Tabela MMU ==================\n";
    outfile << "Endereço Virtual (Binário) -> Endereço Físico\n";
    outfile << "----------------------------------------------\n";

    for (const auto &entry : tabelaMMU)
    {
        outfile << entry.first << " -> " << entry.second << endl;
    }

    outfile << "==============================================\n";
}
