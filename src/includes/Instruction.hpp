#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include "enums/OpcodeEnum.hpp"
#include <ostream>

struct Instruction
{
    Opcode op;
    int Destiny_Register;
    int Register_1;
    int Register_2;

    Instruction(Opcode opc, int rd, int r1, int r2);
    Instruction();

    bool operator==(const Instruction &other) const
    {
        return op == other.op &&
               Destiny_Register == other.Destiny_Register &&
               Register_1 == other.Register_1 &&
               Register_2 == other.Register_2;
    }

    friend std::ostream &operator<<(std::ostream &os, const Instruction &instr);
};

// Definição de um hash para ser usado no unordered_map
namespace std
{
    template <>
    struct hash<Instruction>
    {
        size_t operator()(const Instruction &instr) const
        {
            return ((size_t)instr.op << 24) ^
                   ((size_t)instr.Destiny_Register << 16) ^
                   ((size_t)instr.Register_1 << 8) ^
                   ((size_t)instr.Register_2);
        }
    };
}

#endif
