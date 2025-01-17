#ifndef POLITICAS_ESCALONAMENTO_HPP
#define POLITICAS_ESCALONAMENTO_HPP

#include "../PCB.hpp"
#include <fstream>
#include <queue>
#include <vector>

using namespace std;

// Enum para definir as políticas de escalonamento
enum class PoliticasEscalonamento
{
    FCFS,
    SJF,
    PRIORIDADE,
};

// Classe para implementar as diferentes políticas de escalonamento
class PoliticasEscalonamentoHandler
{
public:
    static PCB *selecionarProcessoFCFS(queue<PCB *> &filaProntos, ofstream &outfile);
    static PCB *selecionarProcessoSJF(queue<PCB *> &filaProntos, ofstream &outfile);
    static PCB *selecionarProcessoPrioridade(queue<PCB *> &filaProntos, ofstream &outfile);
};

#endif
