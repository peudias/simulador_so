#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include "enums/OpcodeEnum.hpp"
#include "Disco.hpp"
#include "Instruction.hpp"
#include "Registers.hpp"
#include "ULA.hpp"
#include "RAM.hpp"
#include "InstructionDecode.hpp"
#include "PCB.hpp"

#include <string>
#include <vector>

class Pipeline
{
private:
    ULA ula;
    std::vector<int> pipelineState;

public:
    Pipeline();
    void PipelineProcess(const Instruction &instr, Registers &regs, RAM &ram, int &PC, Disco &Disco, int &Clock);
    Instruction InstructionFetch(RAM &ram, int endereco, PCB *pcb);
    void Wb(const DecodedInstruction &decoded, int &resultado, RAM &ram, Disco &disco, int &Clock);
    void MemoryAccess(const DecodedInstruction &decoded, int resultado, Registers &regs, int &Clock);
    void Execute(const DecodedInstruction &decoded, Registers &regs, RAM &ram, int &PC, Disco &Disco, int &Clock);

    std::vector<int> getPipelineState() const;
};

#endif
