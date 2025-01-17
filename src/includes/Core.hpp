#ifndef CORE_HPP
#define CORE_HPP

#include "Disco.hpp"
#include "Escalonador.hpp"
#include "Instruction.hpp"
#include "PCB.hpp"
#include "Pipeline.hpp"
#include "RAM.hpp"
#include "Registers.hpp"
#include "UnidadeControle.hpp"

#include <thread>
#include <vector>
#include <sstream>
#include <filesystem>
#include <chrono>
#include <iomanip>

using namespace std;

class Core
{
public:
    int PC;
    int Clock;
    int instructionAddress;

    Registers regs;
    UnidadeControle uc;
    RAM &ram;
    Disco &disco;
    Escalonador &escalonador;
    Pipeline pipeline;

    double tempoTotalEspera = 0.0;
    double tempoTotalRetorno = 0.0;
    int processosExecutados = 0;
    int tempoAtual = 0; // Tempo total já decorrido neste núcleo

    Core(RAM &ram, Disco &disco, Escalonador &escalonador);
    void activate(ofstream &outfile);
    void run();
    void exibirTempoCore(ofstream &outfile);

private:
    chrono::time_point<chrono::high_resolution_clock> inicioExecucao;
    chrono::time_point<chrono::high_resolution_clock> fimExecucao;
    double tempoOcupado = 0.0;
    double tempoOcioso = 0.0;

    void validateMemoryAccess(PCB *processo, int endereco, ofstream &outfile);
    void gerenciarRecursos(PCB *processo, ofstream &outfile);
};

#endif