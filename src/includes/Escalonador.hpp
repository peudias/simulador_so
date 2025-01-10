#ifndef ESCALONADOR_HPP
#define ESCALONADOR_HPP

#include "PCB.hpp"
#include "enums/PoliticasEscalonamento.hpp"

#include <queue>
#include <vector>
#include <mutex>
#include <functional>

using namespace std;

// Comparador para SJF
struct SJFComparator
{
    bool operator()(PCB *a, PCB *b)
    {
        return a->tempoEstimado > b->tempoEstimado;
    }
};

class Escalonador
{
private:
    queue<PCB *> filaProntos;
    queue<PCB *> filaBloqueados;
    mutex mtx;

public:
    Escalonador();
    void adicionarProcesso(PCB *processo, ofstream &outfile); // Adiciona um processo à fila
    PCB *obterProximoProcesso(ofstream &outfile);             // Retorna o próximo processo a ser executado

    void bloquearProcesso(PCB *processo, ofstream &outfile);
    void desbloquearProcessos(ofstream &outfile);

    bool filaVazia() const;
    bool temProcessosProntos() const;
};

#endif
