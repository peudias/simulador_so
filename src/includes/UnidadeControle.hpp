#ifndef UNIDADECONTROLE_HPP
#define UNIDADECONTROLE_HPP

#include "Disco.hpp"
#include "Instruction.hpp"
#include "PCB.hpp"
#include "Pipeline.hpp"
#include "RAM.hpp"
#include "Registers.hpp"

#include <iostream>

using namespace std;

class UnidadeControle
{
private:
    Pipeline pipeline;

public:
    void executarInstrucao(const Instruction &instr, Registers &regs, RAM &ram, int &PC, Disco &disco, int &Clock, PCB &pcb, ofstream &outfile);
};

#endif