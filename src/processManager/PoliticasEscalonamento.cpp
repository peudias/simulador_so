#include "../includes/enums/PoliticasEscalonamento.hpp"

PCB *PoliticasEscalonamentoHandler::selecionarProcessoFCFS(queue<PCB *> &filaProntos, ofstream &outfile)
{
    PCB *processo = filaProntos.front();
    filaProntos.pop();
    outfile << "\n************************************************************************************************************************\n";
    outfile << "[Escalonador][FCFS] Retirando o processo " << processo->pid << " da fila de prontos.\n";
    return processo;
}

PCB *PoliticasEscalonamentoHandler::selecionarProcessoSJF(queue<PCB *> &filaProntos, ofstream &outfile)
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

PCB *PoliticasEscalonamentoHandler::selecionarProcessoPrioridade(queue<PCB *> &filaProntos, ofstream &outfile)
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

PCB *PoliticasEscalonamentoHandler::selecionarProcessoRoundRobin(queue<PCB *> &filaProntos, ofstream &outfile)
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
