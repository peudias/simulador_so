#ifndef RAM_HPP
#define RAM_HPP

#include <iostream>
#include <vector>
#include "Instruction.hpp"
#include "InstructionDecode.hpp"
#include <mutex>

class RAM
{
private:
    std::mutex mtx;

public:
    std::vector<int> memoria;
    std::vector<Instruction> instruction_memory;
    static const int tamanho = 32;

    RAM();

    void write(int endereco, int valor);
    int read(int endereco);

    void writeInstruction(int endereco, const Instruction &instr);
    Instruction fetchInstruction(int endereco) const;

    bool isReserved(int endereco) const;
    void display(ofstream &outfile) const;
    void displayI() const;
};

#endif
