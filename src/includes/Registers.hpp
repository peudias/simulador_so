#ifndef REGISTERS_HPP
#define REGISTERS_HPP
#define NUM_REGISTRADORES 32

#include <iostream>
#include <vector>
#include <utility> // Para std::pair
#include <fstream>

using namespace std;

class Registers
{
private:
    vector<pair<int, bool>> registradores;

public:
    Registers();

    int get(int index) const;
    void set(int index, int value);
    void display(ofstream &outfile) const;

    void reset(); // Reinicializa todos os registradores
    const vector<pair<int, bool>> &getAllRegisters() const;
    void setAllRegisters(const vector<pair<int, bool>> &newRegisters);
};

#endif
