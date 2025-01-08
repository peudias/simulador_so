#include "../includes/Escalonador.hpp"

Escalonador::Escalonador() {}

PCB *Escalonador::obterProximoProcesso(ofstream &outfile)
{
    lock_guard<mutex> lock(mtx);
    if (!filaVazia())
    {
        PCB *processo = filaProntos.front();
        filaProntos.pop();
        outfile << "\n************************************************************************************************************************\n";
        outfile << "[Escalonador] Retirando o processo " << processo->pid << " da fila de prontos.\n";
        return processo;
    }
    outfile << "[Escalonador] Nenhum processo disponível na fila de prontos.\n";
    return nullptr;
}

void Escalonador::adicionarProcesso(PCB *processo, ofstream &outfile)
{
    lock_guard<mutex> lock(mtx);
    filaProntos.push(processo);
    outfile << "[Escalonador] Processo " << processo->pid << " adicionado à fila de prontos.\n";
}

void Escalonador::bloquearProcesso(PCB *processo, ofstream &outfile)
{
    lock_guard<mutex> lock(mtx);
    processo->atualizarEstado(BLOQUEADO, outfile);
    filaBloqueados.push(processo);
    outfile << "[Escalonador] Processo " << processo->pid << " movido para a fila de bloqueados.\n";
}

void Escalonador::desbloquearProcessos(ofstream &outfile)
{
    lock_guard<mutex> lock(mtx);
    while (!filaBloqueados.empty())
    {
        PCB *processo = filaBloqueados.front();
        filaBloqueados.pop();
        processo->atualizarEstado(PRONTO, outfile);
        filaProntos.push(processo);
        outfile << "[Escalonador] Processo " << processo->pid << " desbloqueado e movido para a fila de prontos.\n";
    }
}

bool Escalonador::filaVazia() const
{
    return filaProntos.empty();
}

bool Escalonador::temProcessosProntos() const
{
    return !filaProntos.empty();
}