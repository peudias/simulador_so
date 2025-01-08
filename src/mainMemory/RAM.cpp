#include "../includes/RAM.hpp"

RAM::RAM() : memoria(tamanho, 0), instruction_memory(tamanho) {}

void RAM::write(int endereco, int valor)
{
    std::lock_guard<std::mutex> lock(mtx); // Bloqueia o mutex enquanto a função é executada
    if (endereco >= 0 && endereco < tamanho)
    {
        memoria[endereco] = valor;
    }
    else
    {
        std::cout << "Erro: Endereço fora da memória!" << std::endl;
    }
}

int RAM::read(int endereco)
{
    std::lock_guard<std::mutex> lock(mtx); // Bloqueia o mutex enquanto a função é executada
    if (endereco >= 0 && endereco < tamanho)
    {
        return memoria[endereco];
    }
    else
    {
        std::cout << "Erro: Endereço fora da memória!" << std::endl;
        return -1;
    }
}

void RAM::writeInstruction(int endereco, const Instruction &instr)
{
    std::lock_guard<std::mutex> lock(mtx); // Bloqueia o mutex enquanto a função é executada
    if (endereco >= 0 && endereco < tamanho)
    {
        instruction_memory[endereco] = instr;
    }
    else
    {
        std::cout << "Erro: Endereço inválido para instrução na RAM " << endereco << std::endl;
    }
}

Instruction RAM::fetchInstruction(int endereco) const
{
    if (endereco >= 0 && endereco < tamanho)
    {
        return instruction_memory[endereco];
    }
    std::cout << "Erro: Endereço inválido para instrução na RAM " << endereco << std::endl;
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
        std::cout << "Endereço " << i << " -> " << instruction_memory[i] << std::endl;
    }
}
