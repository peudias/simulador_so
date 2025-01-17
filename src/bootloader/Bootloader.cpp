#include "../includes/Bootloader.hpp"

// Variáveis para armazenar as configurações
string Bootloader::OUTPUT_LOGS_DIR = "";
int Bootloader::NUM_NUCLEOS = 0;
int Bootloader::QUANTUM_PROCESS_MIN = 0;
int Bootloader::QUANTUM_PROCESS_MAX = 0;
PoliticasEscalonamento Bootloader::POLITICA_ESCALONAMENTO = PoliticasEscalonamento::FCFS;

void Bootloader::loadConfigBootloader(const string &file)
{
    ifstream inputFile(file);
    if (!inputFile.is_open())
    {
        cerr << "Erro ao abrir o arquivo de configuração: " << file << endl;
        exit(EXIT_FAILURE);
    }

    unordered_map<string, string> configs;
    string linha;

    while (getline(inputFile, linha))
    {
        size_t igual = linha.find('=');
        if (igual != string::npos)
        {
            string chave = linha.substr(0, igual);
            string valor = linha.substr(igual + 1);
            configs[chave] = valor;
        }
    }

    inputFile.close();

    if (configs.find("NUM_NUCLEOS") == configs.end() || configs.find("OUTPUT_LOGS_DIR") == configs.end() ||
        configs.find("QUANTUM_PROCESS_MIN") == configs.end() || configs.find("QUANTUM_PROCESS_MAX") == configs.end() ||
        configs.find("POLITICA_ESCALONAMENTO") == configs.end())
    {
        cerr << "Arquivo de configuração ('data/configBootloader.txt') inválido: chaves necessárias estão faltando ou definidas incorretamente." << endl;
        exit(EXIT_FAILURE);
    }

    NUM_NUCLEOS = stoi(configs["NUM_NUCLEOS"]);
    OUTPUT_LOGS_DIR = configs["OUTPUT_LOGS_DIR"];
    QUANTUM_PROCESS_MIN = stoi(configs["QUANTUM_PROCESS_MIN"]);
    QUANTUM_PROCESS_MAX = stoi(configs["QUANTUM_PROCESS_MAX"]);

    // Mapeando a política de escalonamento
    unordered_map<string, PoliticasEscalonamento> politicaMap = {
        {"FCFS", PoliticasEscalonamento::FCFS},
        {"SJF", PoliticasEscalonamento::SJF},
        {"PRIORIDADE", PoliticasEscalonamento::PRIORIDADE}};

    string politicaStr = configs["POLITICA_ESCALONAMENTO"];
    if (politicaMap.find(politicaStr) != politicaMap.end())
    {
        POLITICA_ESCALONAMENTO = politicaMap[politicaStr];
    }
    else
    {
        cerr << "Política de escalonamento inválida: " << politicaStr << endl;
        exit(EXIT_FAILURE);
    }
}

void Bootloader::garantirDiretorioSaidaExiste(const string &path)
{
    if (!fs::exists(path))
    {
        try
        {
            fs::create_directories(path);
            cout << "Diretório criado: " << path << endl;
        }
        catch (const fs::filesystem_error &e)
        {
            cerr << "Erro ao criar diretório: " << e.what() << endl;
            exit(EXIT_FAILURE);
        }
    }
}

vector<PCB *> Bootloader::createAndConfigPCBs(Disco &disco, RAM &ram, Registers &regs, Escalonador &escalonador, const vector<string> &arquivosInstrucoes, ofstream &globalLog)
{
    // Criando PCBs
    vector<PCB *> pcbs = ProcessManager::createPCBs(disco, ram, regs, arquivosInstrucoes);

    // Alocação de memória para cada processo && Adicionando os processos ao escalonador
    for (auto &pcb : pcbs)
    {
        int enderecoBase = pcb->getEnderecoBaseInstrucoes();
        int limite = pcb->getLimiteInstrucoes();

        pcb->alocarMemoria(ram, enderecoBase, limite);

        if (POLITICA_ESCALONAMENTO == PoliticasEscalonamento::SJF)
        {
            globalLog << endl
                      << "[Bootloader][SJF] Processo " << pcb->pid << " com tamanho de " << pcb->tempoEstimado << "." << endl;
        }

        if (POLITICA_ESCALONAMENTO == PoliticasEscalonamento::PRIORIDADE)
        {
            globalLog << "[Bootloader][Prioridade] Processo " << pcb->pid << " configurado com prioridade: [Nível " << pcb->prioridade << "][";
            for (int j = 0; j < pcb->prioridade; ++j)
            {
                globalLog << "★";
            }
            globalLog << "]" << endl;
        }

        // Associar recurso apenas ao processo com PID = 2
        if (pcb->pid == 2)
        {
            string recursoNecessario = "impressora";       // Recurso necessário
            pcb->associarRecurso(recursoNecessario, true); // Aloca o recurso
            globalLog << "[Bootloader] Recurso " << recursoNecessario << " alocado ao Processo " << pcb->pid << ".\n";
        }

        escalonador.adicionarProcesso(pcb, globalLog);
    }

    return pcbs;
}

void Bootloader::createCores(vector<Core> &cores, int numNucleos, RAM &ram, Disco &disco, Escalonador &escalonador)
{
    // Criando múltiplos núcleos
    for (int i = 0; i < numNucleos; ++i)
    {
        cores.push_back(Core(ram, disco, escalonador));
    }
}

void Bootloader::inicializarSistema(vector<Core> &cores, Disco &disco, Escalonador &escalonador, Perifericos &periferico, RAM &ram, Registers &regs)
{
    // Garantindo que o diretório de saída exista ("data/logs")
    garantirDiretorioSaidaExiste(OUTPUT_LOGS_DIR);

    // Criando arquivo de log global ("data/global_log.txt")
    ofstream globalLog(string(OUTPUT_LOGS_DIR) + "/global_log.txt", ios::out);
    if (!globalLog.is_open())
    {
        cerr << "Erro ao abrir o arquivo de log global.\n";
        return;
    }

    globalLog << "Inicializando o sistema..." << endl;

    // Configurando os registradores
    disco.setRegistersFromFile(regs, "data/setRegisters.txt");

    // Lista de arquivos de instrução
    vector<string> arquivosInstrucoes = disco.listInstructionsFile("data/instr");

    // Log para Estado Inical do SO
    globalLog << endl
              << "====== Informações Gerais do Sistema Operacional ======" << endl;
    globalLog << "Número de Núcleos: " << NUM_NUCLEOS << endl;
    globalLog << "Número de Processos: " << disco.listInstructionsFile("data/instr").size() << endl;
    globalLog << "Política de Escalonamento: " << (POLITICA_ESCALONAMENTO == PoliticasEscalonamento::FCFS ? "FCFS" : POLITICA_ESCALONAMENTO == PoliticasEscalonamento::SJF ? "SJF"
                                                                                                                                                                           : "PRIORIDADE")
              << endl;
    globalLog << "Recursos Disponíveis: " << endl;
    periferico.exibirPerifericos(globalLog);
    globalLog << "=======================================================" << endl
              << endl;

    // Configurando a política de escalonamento
    escalonador.configurarPolitica(POLITICA_ESCALONAMENTO);

    // Criando e configurando PCBs
    vector<PCB *> pcbs = Bootloader::createAndConfigPCBs(disco, ram, regs, escalonador, arquivosInstrucoes, globalLog);

    // Criando múltiplos núcleos
    createCores(cores, NUM_NUCLEOS, ram, disco, escalonador);

    // Medindo o tempo total de execução
    auto inicio = chrono::high_resolution_clock::now();

    // Executando os núcleos em threads
    vector<thread> threads;
    for (auto &core : cores)
    {
        threads.push_back(thread(&Core::run, &core)); // Rodando cada núcleo em uma thread
    }

    // Aguardando que todas as threads terminem
    for (auto &th : threads)
    {
        th.join(); // Espera todas as threads terminarem
    }

    // Medindo o tempo final de execução
    auto fim = chrono::high_resolution_clock::now();
    double duracao = chrono::duration_cast<chrono::duration<double, milli>>(fim - inicio).count();

    globalLog << endl
              << "=============== Tempo Total de Execução ===============" << endl;
    globalLog << "Duração Total: " << fixed << setprecision(3) << duracao << " ms\n";

    // Exibindo estatísticas de cada núcleo
    for (size_t i = 0; i < cores.size(); ++i)
    {
        globalLog << endl
                  << "====================== [CORE " << i + 1 << "] =======================";
        cores[i].exibirTempoCore(globalLog);
    }

    // Exibindo o estado final da RAM
    globalLog << "\n================= Estado Final da RAM =================\n";
    ram.display(globalLog);

    // Exibindo o estado final do Disco
    globalLog << "\n================ Estado Final do Disco ================\n";
    disco.display(globalLog);

    // Liberando memória dos processos
    for (auto &pcb : pcbs)
    {
        pcb->liberarMemoria(ram);
        delete pcb;
    }

    cout << "Simulação finalizada. Logs disponíveis em: " << OUTPUT_LOGS_DIR << endl;

    globalLog.close();
}
