#include "../includes/RAM.hpp"

RAM::RAM() : memoria(tamanho, 0), instruction_memory(tamanho) {}

void RAM::write(int endereco, int valor)
{
    lock_guard<mutex> lock(mtx); // Bloqueia o mutex enquanto a função é executada
    if (endereco >= 0 && endereco < tamanho)
    {
        memoria[endereco] = valor;
    }
    else
    {
        cout << "Erro: Endereço fora da memória!" << endl;
    }
}

int RAM::read(int endereco)
{
    lock_guard<mutex> lock(mtx); // Bloqueia o mutex enquanto a função é executada
    if (endereco >= 0 && endereco < tamanho)
    {
        return memoria[endereco];
    }
    else
    {
        cout << "Erro: Endereço fora da memória!" << endl;
        return -1;
    }
}

void RAM::writeInstruction(int endereco, const Instruction &instr)
{
    lock_guard<mutex> lock(mtx); // Bloqueia o mutex enquanto a função é executada
    if (endereco >= 0 && endereco < tamanho)
    {
        instruction_memory[endereco] = instr;
    }
    else
    {
        cout << "Erro: Endereço inválido para instrução na RAM " << endereco << endl;
    }
}

Instruction RAM::fetchInstruction(int endereco) const
{
    if (endereco >= 0 && endereco < tamanho)
    {
        return instruction_memory[endereco];
    }
    cout << "Erro: Endereço inválido para instrução na RAM " << endereco << endl;
    return Instruction(ADD, 0, 0, 0);
}

bool RAM::isReserved(int endereco) const
{
    if (endereco < 0 || endereco >= tamanho)
    {
        return true; // Fora do limite, considerado reservado
    }
    return memoria[endereco] != 0; // Qualquer valor diferente de 0 indica que está reservado
}

void RAM::display(ofstream &outfile) const
{
    for (int i = 0; i < tamanho; i++)
    {
        outfile << "Endereço " << i << " -> " << memoria[i] << endl;
    }
}

void RAM::displayI() const
{
    for (int i = 0; i < tamanho; i++)
    {
        cout << "Endereço " << i << " -> " << instruction_memory[i] << endl;
    }
}
