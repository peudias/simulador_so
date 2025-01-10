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

    if (politicaAtual == PoliticasEscalonamento::FCFS)
    {
        processoSelecionado = filaProntos.front();
        filaProntos.pop();
        outfile << "\n************************************************************************************************************************\n";
        outfile << "[Escalonador][FCFS] Retirando o processo " << processoSelecionado->pid << " da fila de prontos." << endl;
    }
    else if (politicaAtual == PoliticasEscalonamento::SJF)
    {
        // Encontrar o processo com menor quantum
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

        // Recolocar todos os processos na fila, exceto o selecionado
        for (PCB *pcb : tempoQueue)
        {
            if (pcb != processoMenorTempo)
            {
                filaProntos.push(pcb);
            }
        }

        processoSelecionado = processoMenorTempo;

        outfile
            << "\n************************************************************************************************************************\n";
        outfile << "[Escalonador][SJF] Retirando o processo " << processoMenorTempo->pid << " da fila de prontos. Quantum: " << processoMenorTempo->quantumProcesso << "." << endl;
    }
    else if (politicaAtual == PoliticasEscalonamento::PRIORIDADE)
    {
        // Encontrar o processo com maior prioridade
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

        // Recolocar todos os processos na fila, exceto o selecionado
        for (PCB *pcb : prioridadeQueue)
        {
            if (pcb != processoMaiorPrioridade)
            {
                filaProntos.push(pcb);
            }
        }

        processoSelecionado = processoMaiorPrioridade;

        outfile << "\n************************************************************************************************************************\n";
        outfile << "[Escalonador][Prioridade] Retirando o processo " << processoMaiorPrioridade->pid
                << " da fila de prontos. Prioridade [Nível " << processoMaiorPrioridade->prioridade << "][";

        for (int j = 0; j < processoMaiorPrioridade->prioridade; ++j)
        {
            outfile << "★";
        }
        outfile << "]" << endl;
    }
    else if (politicaAtual == PoliticasEscalonamento::ROUNDROBIN)
    {
        // Selecionar o próximo processo na fila circular
        processoSelecionado = filaProntos.front();
        filaProntos.pop();

        // Verificar se o processo foi finalizado
        if (processoSelecionado->verificarEstado(FINALIZADO))
        {
            outfile << "[Escalonador][RoundRobin] Processo " << processoSelecionado->pid << " já finalizado. Removendo da fila." << endl;
            return processoSelecionado; // Não reinserir processos finalizados
        }

        // Decrementa o quantum do processo
        processoSelecionado->decrementarQuantum(outfile);

        // Se o quantum expirou, verifica se o processo terminou
        if (processoSelecionado->quantumExpirado())
        {
            if (processoSelecionado->verificarEstado(FINALIZADO))
            {
                outfile << "[Escalonador][RoundRobin] Processo " << processoSelecionado->pid << " finalizado." << endl;
                return processoSelecionado; // Processo finalizado, não reinserir na fila
            }
            else
            {
                processoSelecionado->resetarQuantum(outfile);
                outfile << "[Escalonador][RoundRobin] Quantum do processo " << processoSelecionado->pid
                        << " expirou. Resetando para " << processoSelecionado->quantumProcesso << " ms." << endl;
            }
        }

        // Reinsere o processo na fila, se não finalizado
        filaProntos.push(processoSelecionado);
        return processoSelecionado;
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