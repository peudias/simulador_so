#ifndef RAM_HPP
#define RAM_HPP

#include <iostream>
#include <vector>
#include "Instruction.hpp"
#include "InstructionDecode.hpp"
#include <mutex>

using namespace std;

class RAM
{
private:
    mutex mtx;

public:
    vector<int> memoria;
    vector<Instruction> instruction_memory;
    static const int tamanho = 256;

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
