#include "../includes/Registers.hpp"

Registers::Registers() : registradores(NUM_REGISTRADORES, make_pair(0, false)) {}

int Registers::get(int index) const
{
    if (index >= 0 && index < registradores.size())
    {
        return registradores[index].first;
    }
    cerr << "Erro: Registrador inválido " << index << endl;
    return 0;
}

void Registers::set(int index, int value)
{
    if (index >= 0 && index < registradores.size())
    {
        registradores[index] = {value, true};
    }
    else
    {
        cerr << "Erro: Registrador inválido " << index << endl;
    }
}

void Registers::display(ofstream &outfile) const
{
    for (size_t i = 0; i < registradores.size(); ++i)
    {
        outfile << "R[" << i << "] = " << registradores[i].first << ", Flag = " << (registradores[i].second ? "sujo" : "limpo") << endl;
    }
}

// Método para resetar os registradores
void Registers::reset()
{
    for (size_t i = 0; i < registradores.size(); ++i)
    {
        registradores[i].first = 0;      // Resetando o valor dos registradores para 0
        registradores[i].second = false; // Resetando a flag de "sujo" para "limpo"
    }
}

// Métodos para manipular todos os registradores
const vector<pair<int, bool>> &Registers::getAllRegisters() const
{
    return registradores;
}

void Registers::setAllRegisters(const vector<pair<int, bool>> &newRegisters)
{
    if (newRegisters.size() == registradores.size())
    {
        registradores = newRegisters;
    }
    else
    {
        cerr << "Erro: O número de registradores não corresponde." << endl;
    }
}