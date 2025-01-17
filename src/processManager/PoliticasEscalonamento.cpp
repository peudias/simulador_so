#include "../includes/enums/PoliticasEscalonamento.hpp"

PCB *PoliticasEscalonamentoHandler::selecionarProcessoFCFS(queue<PCB *> &filaProntos, ofstream &outfile)
{
    static int tempoAtualFCFS = 0;
    static double tempoTotalEsperaFCFS = 0;
    static double tempoTotalRetornoFCFS = 0;
    static int processosExecutadosFCFS = 0;

    PCB *processo = filaProntos.front();
    filaProntos.pop();

    int tempoEsperaFCFS = tempoAtualFCFS;
    int tempoRetornoFCFS = tempoEsperaFCFS + processo->tempoEstimado;

    tempoTotalEsperaFCFS += tempoEsperaFCFS;
    tempoTotalRetornoFCFS += tempoRetornoFCFS;
    tempoAtualFCFS += processo->tempoEstimado;
    processosExecutadosFCFS++;

    processo->custoComputacional = tempoTotalRetornoFCFS / processosExecutadosFCFS;

    outfile
        << "\n************************************************************************************************************************\n";
    outfile << "[Escalonador][FCFS] Retirando o processo " << processo->pid << " da fila de PRONTOS. Tamanho: " << processo->tempoEstimado << ".\n";
    outfile << "[Escalonador][FCFS] Tempo de espera: " << tempoEsperaFCFS << " | Tempo de retorno: " << tempoRetornoFCFS << ".\n";
    outfile << "[Escalonador][FCFS] Tempo médio de espera: " << (tempoTotalEsperaFCFS / processosExecutadosFCFS)
            << " | Tempo médio de retorno: " << (tempoTotalRetornoFCFS / processosExecutadosFCFS) << ".\n";
    outfile << "[Escalonador][FCFS] Quantidade de processos executados no simulador: " << processosExecutadosFCFS << ".\n";

    return processo;
}

PCB *PoliticasEscalonamentoHandler::selecionarProcessoSJF(queue<PCB *> &filaProntos, ofstream &outfile)
{
    vector<PCB *> tempoQueue;
    PCB *processoMenorTempo = nullptr;
    static int tempoAtual = 0;
    static double tempoTotalEspera = 0;
    static double tempoTotalRetorno = 0;
    static int processosExecutados = 0;

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

    int tempoEspera = tempoAtual;
    int tempoRetorno = tempoEspera + processoMenorTempo->tempoEstimado;

    tempoTotalEspera += tempoEspera;
    tempoTotalRetorno += tempoRetorno;

    tempoAtual += processoMenorTempo->tempoEstimado;
    processosExecutados++;

    processoMenorTempo->custoComputacional = tempoTotalRetorno / processosExecutados;

    outfile << "\n************************************************************************************************************************\n";
    outfile << "[Escalonador][SJF] Retirando o processo " << processoMenorTempo->pid
            << " da fila de PRONTOS. Tamanho: " << processoMenorTempo->tempoEstimado << ".\n";
    outfile << "[Escalonador][SJF] Tempo de espera: " << tempoEspera << " | Tempo de retorno: " << tempoRetorno << ".\n";
    outfile << "[Escalonador][SJF] Tempo médio de espera: " << (tempoTotalEspera / processosExecutados)
            << " | Tempo médio de retorno: " << (tempoTotalRetorno / processosExecutados) << ".\n";
    outfile << "[Escalonador][SJF] Quantidade de processos executados no simulador: " << processosExecutados << ".\n";

    return processoMenorTempo;
}

PCB *PoliticasEscalonamentoHandler::selecionarProcessoPrioridade(queue<PCB *> &filaProntos, ofstream &outfile)
{
    vector<PCB *> prioridadeQueue;
    PCB *processoMaiorPrioridade = nullptr;
    static int tempoAtualPrioridade = 0;
    static double tempoTotalEsperaPrioridade = 0;
    static double tempoTotalRetornoPrioridade = 0;
    static int processosExecutadosPrioridade = 0;

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

    int tempoEsperaPrioridade = tempoAtualPrioridade;
    int tempoRetornoPrioridade = tempoEsperaPrioridade + processoMaiorPrioridade->tempoEstimado;

    tempoTotalEsperaPrioridade += tempoEsperaPrioridade;
    tempoTotalRetornoPrioridade += tempoRetornoPrioridade;
    tempoAtualPrioridade += processoMaiorPrioridade->tempoEstimado;
    processosExecutadosPrioridade++;

    processoMaiorPrioridade->custoComputacional = tempoTotalRetornoPrioridade / processosExecutadosPrioridade;

    outfile << "\n************************************************************************************************************************\n";
    outfile << "[Escalonador][Prioridade] Retirando o processo " << processoMaiorPrioridade->pid
            << " da fila de PRONTOS. Prioridade [Nível " << processoMaiorPrioridade->prioridade << "][";
    for (int j = 0; j < processoMaiorPrioridade->prioridade; ++j)
    {
        outfile << "★";
    }
    outfile << "]\n";
    outfile << "[Escalonador][Prioridade] Tempo de espera: " << tempoEsperaPrioridade << " | Tempo de retorno: " << tempoRetornoPrioridade << ".\n";
    outfile << "[Escalonador][Prioridade] Tempo médio de espera: " << (tempoTotalEsperaPrioridade / processosExecutadosPrioridade)
            << " | Tempo médio de retorno: " << (tempoTotalRetornoPrioridade / processosExecutadosPrioridade) << ".\n";
    outfile << "[Escalonador][Prioridade] Quantidade de processos executados no simulador: " << processosExecutadosPrioridade << ".\n";

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
