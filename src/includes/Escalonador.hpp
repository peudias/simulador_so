#ifndef ESCALONADOR_HPP
#define ESCALONADOR_HPP

#include "PCB.hpp"
#include "enums/PoliticasEscalonamento.hpp"
#include <queue>
#include <mutex>
#include <fstream>

using namespace std;

class Escalonador
{
private:
    queue<PCB *> filaProntos;
    queue<PCB *> filaBloqueados;
    PoliticasEscalonamento politicaAtual;
    mutex mtx;
    Cache *cache;
    RAM *ram;

public:
    queue<PCB *> &getFilaProntos();
    Escalonador(PoliticasEscalonamento politica = PoliticasEscalonamento::FCFS);
    void configurarCacheERAM(Cache *cache, RAM *ram);

    void configurarPolitica(PoliticasEscalonamento novaPolitica);
    void adicionarProcesso(PCB *processo, ofstream &outfile); // Adiciona um processo à fila
    PCB *obterProximoProcesso(ofstream &outfile);
    void bloquearProcesso(PCB *processo, ofstream &outfile);
    void desbloquearProcessos(ofstream &outfile);

    bool filaVazia() const;
    bool temProcessosProntos() const;
};

#endif
