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
    outfile << "[Escalonador] Processo " << processo->pid << " adicionado à Fila de PRONTOS" << endl;
}

PCB *Escalonador::obterProximoProcesso(ofstream &outfile)
{
    lock_guard<mutex> lock(mtx);

    if (filaVazia())
    {
        outfile << "[Escalonador] Nenhum processo disponível na fila de PRONTOS" << endl;
        return nullptr;
    }

    PCB *processoSelecionado = nullptr;

    switch (politicaAtual)
    {
    case PoliticasEscalonamento::FCFS:
        processoSelecionado = PoliticasEscalonamentoHandler::selecionarProcessoFCFS(filaProntos, outfile);
        break;
    case PoliticasEscalonamento::SJF:
        processoSelecionado = PoliticasEscalonamentoHandler::selecionarProcessoSJF(filaProntos, outfile);
        break;
    case PoliticasEscalonamento::PRIORIDADE:
        processoSelecionado = PoliticasEscalonamentoHandler::selecionarProcessoPrioridade(filaProntos, outfile);
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
        outfile << "[Escalonador] Processo " << processo->pid << " Desbloqueado e Movido para a Fila de PRONTOS" << endl;
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
