#ifndef BOOTLOADER_HPP
#define BOOTLOADER_HPP

#include "Core.hpp"
#include "Disco.hpp"
#include "Escalonador.hpp"
#include "Perifericos.hpp"
#include "RAM.hpp"
#include "Registers.hpp"
#include "ProcessManager.hpp"
#include "Cache.hpp"

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <filesystem>
#include <thread>
#include <chrono>

using namespace std;
namespace fs = filesystem;

class Bootloader
{
public:
    static string OUTPUT_LOGS_DIR;
    static int NUM_NUCLEOS;
    static int QUANTUM_PROCESS_MIN;
    static int QUANTUM_PROCESS_MAX;
    static PoliticasEscalonamento POLITICA_ESCALONAMENTO;
    static int CACHE_CAPACIDADE;
    static Cache *cache;

    static void loadConfigBootloader(const string &file);
    static void garantirDiretorioSaidaExiste(const string &path);
    static void inicializarSistema(vector<Core> &cores, Disco &disco, Escalonador &escalonador, Perifericos &periferico, RAM &ram, Registers &regs);
    static vector<PCB *> createAndConfigPCBs(Disco &disco, RAM &ram, Registers &regs, Escalonador &escalonador, const vector<string> &arquivosInstrucoes, ofstream &log);
    static void createCores(vector<Core> &cores, int numNucleos, RAM &ram, Disco &disco, Escalonador &escalonador);

    static int getQuantumProcessMin() { return QUANTUM_PROCESS_MIN; };
    static int getQuantumProcessMax() { return QUANTUM_PROCESS_MAX; };

    static void liberarRecursos(); // Para liberar a Cache no final da execução
};

#endif