#include "../includes/PCB.hpp"
#include <iostream>
#include <iomanip>

PCB::PCB(int id, int quantum, const Registers &regs, int enderecoBase, int limite)
    : pid(id), estado(PRONTO), PC(0), quantumProcesso(quantum), quantumRestante(quantum), registradores(regs), enderecoBaseInstrucoes(enderecoBase), enderecoLimiteInstrucoes(limite) {}

void PCB::atualizarEstado(EstadoProcesso novoEstado, ofstream &outfile)
{
    estado = novoEstado;
    outfile << "[PCB] Processo " << pid << " alterado para estado: "
            << (novoEstado == PRONTO ? "PRONTO" : novoEstado == EXECUCAO ? "EXECUCAO"
                                              : novoEstado == BLOQUEADO  ? "BLOQUEADO"
                                                                         : "FINALIZADO")
            << std::endl;
}

bool PCB::verificarEstado(EstadoProcesso verEstado) const
{
    return estado == verEstado;
}

void PCB::salvarEstado(const std::vector<int> &pipelineState)
{
    // Salva o estado do pipeline
    estadoPipeline = pipelineState;

    // Salva os registradores no PCB
    std::vector<std::pair<int, bool>> registradoresAtuais = registradores.getAllRegisters();
    registradores.setAllRegisters(registradoresAtuais);

    // Salva o valor atual do PC
    this->PC = PC;

    // std::cout << "Estado completo do processo " << pid << " salvo no PCB.\n";
}

void PCB::restaurarEstado(std::vector<int> &pipelineState, ofstream &outfile)
{
    // Restaura o estado do pipeline
    pipelineState = estadoPipeline;

    // Restaura os registradores do PCB
    const std::vector<std::pair<int, bool>> &estadoRegistradores = registradores.getAllRegisters();
    registradores.setAllRegisters(estadoRegistradores);

    // Restaura o valor do PC
    this->PC = PC;

    // cout << "Estado completo do processo " << pid << " restaurado do PCB.\n";
}

void PCB::decrementarQuantum(ofstream &outfile)
{
    if (quantumRestante > 0)
    {
        quantumRestante--;
        outfile << "[Quantum] Processo " << pid << ", Quantum restante: " << quantumRestante << endl;
    }
}

bool PCB::quantumExpirado() const
{
    return quantumRestante <= 0;
}

void PCB::resetarQuantum(ofstream &outfile)
{
    quantumRestante = quantumProcesso;
    atualizarEstado(BLOQUEADO, outfile);
    outfile << "[Preempção] Quantum do processo " << pid << " foi reiniciado para " << quantumProcesso << "\n";
}

// Gerenciamento de memoria
void PCB::alocarMemoria(RAM &ram, int enderecoBase, int limite)
{
    // Verifica se a memória já está alocada
    for (int i = enderecoBase; i <= limite; ++i)
    {
        if (ram.isReserved(i))
        {
            std::cerr << "Erro: Endereço " << i << " já está reservado.\n";
            return;
        }
    }
    // Aloca na RAM e registra no PCB
    for (int i = enderecoBase; i <= limite; ++i)
    {
        ram.write(i, 0); // Inicializa os endereços na RAM
    }
    memoriaAlocada = {enderecoBase, limite};
    // std::cout << "Memória alocada ao processo " << pid << ": Base=" << enderecoBase << ", Limite=" << limite << "\n";
}

bool PCB::verificarAcessoMemoria(int endereco) const
{
    if (memoriaAlocada.empty())
        return false;
    return endereco >= memoriaAlocada[0] && endereco <= memoriaAlocada[1];
}

void PCB::liberarMemoria(RAM &ram)
{
    if (!memoriaAlocada.empty())
    {
        for (int i = memoriaAlocada[0]; i <= memoriaAlocada[1]; ++i)
        {
            ram.write(i, 0); // Libera os endereços na RAM
        }
        memoriaAlocada.clear();
        // std::cout << "Memória liberada para o processo " << pid << "\n";
    }
}

// Gerenciamento de recursos (I/O)
void PCB::associarRecurso(const std::string &nomeRecurso, bool alocado)
{
    recursos.estadoPeriferico(nomeRecurso, alocado);
    // std::cout << "Recurso " << nomeRecurso << (alocado ? " alocado" : " liberado") << " ao processo " << pid << ".\n";
}

bool PCB::verificarRecurso(const std::string &nomeRecurso) const
{
    return recursos.verificarPeriferico(nomeRecurso);
}

void PCB::exibirPCB(ofstream &outfile) const
{
    outfile << "\n=============== [PCB] ===============" << "\n"
            << "[PCB] Processo ID: " << pid << "\n"
            << "Estado: " << (estado == PRONTO ? "PRONTO" : estado == EXECUCAO ? "EXECUCAO"
                                                        : estado == BLOQUEADO  ? "BLOQUEADO"
                                                                               : "FINALIZADO")
            << "\n"
            << "Quantum Total: " << quantumProcesso << ", Quantum Restante: " << quantumRestante << "\n"
            << "PC: " << PC << "\n"
            << "\nRegistradores:\n";
    registradores.display(outfile);

    outfile << "\nMemória Alocada:\n";
    if (!memoriaAlocada.empty())
    {
        outfile << "  Base: " << memoriaAlocada[0] << ", Limite: " << memoriaAlocada[1] << "\n";
    }
    else
    {
        outfile << "  Nenhuma memória alocada\n";
    }

    outfile << "\nEstado do Pipeline: ";
    if (!estadoPipeline.empty())
    {
        for (const auto &val : estadoPipeline)
        {
            outfile << val << " ";
        }
        outfile << "\n";
    }
    else
    {
        outfile << "  Nenhum estado salvo\n";
    }

    outfile << "\nRecursos Associados:\n";
    recursos.exibirPerifericos(outfile);

    outfile << "\n===============================\n";
}

int PCB::getEnderecoBaseInstrucoes() const
{
    return enderecoBaseInstrucoes;
}

int PCB::getLimiteInstrucoes() const
{
    return enderecoLimiteInstrucoes;
}
