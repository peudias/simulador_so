#ifndef ESCALONADOR_HPP
#define ESCALONADOR_HPP

#include "PCB.hpp"
#include <queue>
#include <vector>
#include <mutex>

using namespace std;

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
