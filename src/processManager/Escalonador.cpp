#include "../includes/Escalonador.hpp"

Escalonador::Escalonador(PoliticasEscalonamento politica)
    : politicaAtual(politica) {}

void Escalonador::configurarPolitica(PoliticasEscalonamento novaPolitica)
{
    lock_guard<mutex> lock(mtx);
    politicaAtual = novaPolitica;
}

void Escalonador::adicionarProcesso(PCB *processo, ofstream &outfile)
{

    lock_guard<mutex> lock(mtx);
    filaProntos.push(processo);
    outfile << "[Escalonador] Processo " << processo->pid << " adicionado à fila de prontos.\n";
}

PCB *Escalonador::obterProximoProcesso(ofstream &outfile)
{
    lock_guard<mutex> lock(mtx);

    if (filaVazia())
    {
        outfile << "[Escalonador] Nenhum processo disponível na fila de prontos." << endl;
        return nullptr;
    }

    PCB *processoSelecionado = nullptr;

    switch (politicaAtual)
    {
    case PoliticasEscalonamento::FCFS:
        processoSelecionado = selecionarProcessoFCFS(outfile);
        break;
    case PoliticasEscalonamento::SJF:
        processoSelecionado = selecionarProcessoSJF(outfile);
        break;
    case PoliticasEscalonamento::PRIORIDADE:
        processoSelecionado = selecionarProcessoPrioridade(outfile);
        break;
    case PoliticasEscalonamento::ROUNDROBIN:
        processoSelecionado = selecionarProcessoRoundRobin(outfile);
        break;
    }

    if (!processoSelecionado)
    {
        outfile << "[Escalonador] Nenhum processo válido encontrado.\n";
    }

    return processoSelecionado;
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

PCB *Escalonador::selecionarProcessoFCFS(ofstream &outfile)
{
    PCB *processo = filaProntos.front();
    filaProntos.pop();
    outfile << "\n************************************************************************************************************************\n";
    outfile << "[Escalonador][FCFS] Retirando o processo " << processo->pid << " da fila de prontos.\n";
    return processo;
}

PCB *Escalonador::selecionarProcessoSJF(ofstream &outfile)
{
    vector<PCB *> tempoQueue;
    PCB *processoMenorTempo = nullptr;

    while (!filaProntos.empty())
    {
        PCB *atual = filaProntos.front();
        filaProntos.pop();
        tempoQueue.push_back(atual);

        if (!processoMenorTempo || atual->quantumProcesso < processoMenorTempo->quantumProcesso)
        {
            processoMenorTempo = atual;
        }
    }

    for (PCB *pcb : tempoQueue)
    {
        if (pcb != processoMenorTempo)
        {
            filaProntos.push(pcb);
        }
    }

    outfile << "\n************************************************************************************************************************\n";
    outfile << "[Escalonador][SJF] Retirando o processo " << processoMenorTempo->pid << " da fila de prontos. Quantum: " << processoMenorTempo->quantumProcesso << ".\n";

    return processoMenorTempo;
}

PCB *Escalonador::selecionarProcessoPrioridade(ofstream &outfile)
{
    vector<PCB *> prioridadeQueue;
    PCB *processoMaiorPrioridade = nullptr;

    while (!filaProntos.empty())
    {
        PCB *atual = filaProntos.front();
        filaProntos.pop();
        prioridadeQueue.push_back(atual);

        if (!processoMaiorPrioridade || atual->prioridade > processoMaiorPrioridade->prioridade)
        {
            processoMaiorPrioridade = atual;
        }
    }

    for (PCB *pcb : prioridadeQueue)
    {
        if (pcb != processoMaiorPrioridade)
        {
            filaProntos.push(pcb);
        }
    }

    outfile << "\n************************************************************************************************************************\n";
    outfile << "[Escalonador][Prioridade] Retirando o processo " << processoMaiorPrioridade->pid
            << " da fila de prontos. Prioridade [Nível " << processoMaiorPrioridade->prioridade << "][";

    for (int j = 0; j < processoMaiorPrioridade->prioridade; ++j)
    {
        outfile << "★";
    }
    outfile << "]\n";

    return processoMaiorPrioridade;
}

PCB *Escalonador::selecionarProcessoRoundRobin(ofstream &outfile)
{
    PCB *processo = filaProntos.front();
    filaProntos.pop();

    if (processo->verificarEstado(FINALIZADO))
    {
        outfile << "[Escalonador][RoundRobin] Processo " << processo->pid << " já finalizado. Removendo da fila.\n";
        return processo;
    }

    processo->decrementarQuantum(outfile);

    if (processo->quantumExpirado())
    {
        if (processo->verificarEstado(FINALIZADO))
        {
            outfile << "[Escalonador][RoundRobin] Processo " << processo->pid << " finalizado.\n";
            return processo;
        }
        else
        {
            processo->resetarQuantum(outfile);
            outfile << "[Escalonador][RoundRobin] Quantum do processo " << processo->pid
                    << " expirou. Resetando para " << processo->quantumProcesso << " ms.\n";
        }
    }

    filaProntos.push(processo);
    return processo;
}
