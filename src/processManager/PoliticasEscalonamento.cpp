#include "../includes/enums/PoliticasEscalonamento.hpp"

PCB *PoliticasEscalonamentoHandler::selecionarProcessoFCFS(queue<PCB *> &filaProntos, ofstream &outfile)
{
    PCB *processo = filaProntos.front();
    filaProntos.pop();

    outfile
        << "\n************************************************************************************************************************\n";
    outfile << "[Escalonador][FCFS] Retirando o processo " << processo->pid << " da fila de PRONTOS. Tamanho: " << processo->tempoEstimado << ".\n";

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

        if (!processoMenorTempo || atual->tempoEstimado < processoMenorTempo->tempoEstimado)
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
    outfile << "[Escalonador][SJF] Retirando o processo " << processoMenorTempo->pid
            << " da fila de PRONTOS. Tamanho: " << processoMenorTempo->tempoEstimado << ".\n";

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
            << " da fila de PRONTOS. Prioridade [Nível " << processoMaiorPrioridade->prioridade << "][";
    for (int j = 0; j < processoMaiorPrioridade->prioridade; ++j)
    {
        outfile << "★";
    }

    return processoMaiorPrioridade;
}

PCB *PoliticasEscalonamentoHandler::selecionarProcessoSimilaridade(queue<PCB *> &filaProntos, Cache &cache, RAM &ram, ofstream &outfile)
{
    // Utiliza o LSH no Bootloader para organizar a fila antes da execução

    // Retorna o primeiro processo da fila reorganizada
    if (filaProntos.empty())
    {
        outfile << "[Erro] Nenhum processo disponível para execução no escalonador de Similaridade.\n";
        return nullptr;
    }

    PCB *processoSelecionado = filaProntos.front();
    filaProntos.pop();

    outfile << "[Escalonador][SIMILARIDADE] Processo " << processoSelecionado->pid
            << " selecionado com LSH.\n";

    return processoSelecionado;
}
