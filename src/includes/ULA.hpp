#ifndef ULA_HPP
#define ULA_HPP

#include "enums/OpcodeEnum.hpp"

#include <iostream>

using namespace std;

class ULA
{
public:
    int exec(int operador_1, int operador_2, Opcode operando);
};

#endif
