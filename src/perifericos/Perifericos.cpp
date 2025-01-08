#include "../includes/Perifericos.hpp"

Perifericos::Perifericos()
{
    perifericos["teclado"] = false;
    perifericos["mouse"] = false;
    perifericos["impressora"] = false;
}

void Perifericos::estadoPeriferico(const std::string &nomeRecurso, bool estado)
{
    if (perifericos.find(nomeRecurso) != perifericos.end())
    {
        perifericos[nomeRecurso] = estado;
        // std::cout << "Periférico " << nomeRecurso << (estado ? " alocado." : " liberado.") << std::endl;
    }
    else
    {
        // std::cerr << "Erro: Periférico " << nomeRecurso << " não encontrado!" << std::endl;
    }
}

bool Perifericos::verificarPeriferico(const std::string &nomeRecurso) const
{
    auto it = perifericos.find(nomeRecurso);
    return it != perifericos.end() && it->second;
}

void Perifericos::exibirPerifericos(ofstream &outfile) const
{
    for (const auto &[recurso, estado] : perifericos)
    {
        outfile << "- " << recurso << ": " << (estado ? "Em uso" : "Disponível") << std::endl;
    }
}
