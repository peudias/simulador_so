#ifndef ESCALONADOR_HPP
#define ESCALONADOR_HPP

#include "PCB.hpp"
#include "enums/PoliticasEscalonamentoEnum.hpp"

#include <queue>
#include <vector>
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

    PCB *selecionarProcessoFCFS(ofstream &outfile);
    PCB *selecionarProcessoSJF(ofstream &outfile);
    PCB *selecionarProcessoPrioridade(ofstream &outfile);
    PCB *selecionarProcessoRoundRobin(ofstream &outfile);

public:
    Escalonador(PoliticasEscalonamento politica = PoliticasEscalonamento::FCFS);

    void configurarPolitica(PoliticasEscalonamento novaPolitica);

    void adicionarProcesso(PCB *processo, ofstream &outfile); // Adiciona um processo à fila
    PCB *obterProximoProcesso(ofstream &outfile);             // Retorna o próximo processo a ser executado

    void bloquearProcesso(PCB *processo, ofstream &outfile);
    void desbloquearProcessos(ofstream &outfile);

    bool filaVazia() const;
    bool temProcessosProntos() const;
};

#endif
