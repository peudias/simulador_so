#ifndef LSH_HPP
#define LSH_HPP

#include "PCB.hpp"
#include "RAM.hpp"
#include <unordered_map>
#include <vector>
#include <queue>

using namespace std;

class LSH
{
public:
    static void organizarPorSimilaridade(queue<PCB *> &filaProntos, RAM &ram, ofstream &outfile);

private:
    static size_t calcularHashLSH(PCB *processo, RAM &ram);
    static int calcularSimilaridade(PCB *p1, PCB *p2, RAM &ram);
};

#endif
