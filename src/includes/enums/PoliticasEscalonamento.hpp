#ifndef POLITICAS_ESCALONAMENTO_HPP
#define POLITICAS_ESCALONAMENTO_HPP

#include "../PCB.hpp"
#include "../Cache.hpp"
#include "../LSH.hpp"
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
    SIMILARIDADE
};

// Classe para implementar as diferentes políticas de escalonamento
class PoliticasEscalonamentoHandler
{
public:
    static PCB *selecionarProcessoFCFS(queue<PCB *> &filaProntos, ofstream &outfile);
    static PCB *selecionarProcessoSJF(queue<PCB *> &filaProntos, ofstream &outfile);
    static PCB *selecionarProcessoPrioridade(queue<PCB *> &filaProntos, ofstream &outfile);
    static PCB *selecionarProcessoSimilaridade(queue<PCB *> &filaProntos, Cache &cache, RAM &ram, ofstream &outfile);
};

#endif
