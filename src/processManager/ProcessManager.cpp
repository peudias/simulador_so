#include "../includes/ProcessManager.hpp"

vector<PCB *> ProcessManager::createPCBs(Disco &disco, RAM &ram, Registers &regs, const vector<string> &arquivosInstrucoes)
{
    vector<PCB *> pcbs;
    int enderecoAtual = 0; // Endereço inicial na RAM

    // Configurando gerador aleatório de números para o quantum
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(Bootloader::QUANTUM_PROCESS_MIN, Bootloader::QUANTUM_PROCESS_MAX);

    // Configurando lista de prioridades (números de 1 até o número total de processos)
    vector<int> prioridades;
    for (int i = 1; i <= arquivosInstrucoes.size(); ++i)
    {
        prioridades.push_back(i);
    }
    // Embaralhar as prioridades
    shuffle(prioridades.begin(), prioridades.end(), gen);

    // Criando pcbs a partir da lista de arquivos de instruções e carregando as instruções na RAM
    for (int i = 0; i < arquivosInstrucoes.size(); ++i)
    {
        int quantidadeInstrucoes = disco.loadInstructionsFromFile(ram, arquivosInstrucoes[i], enderecoAtual);
        if (quantidadeInstrucoes == -1)
        {
            cerr << "Erro ao quantificar instruções do arquivo: " << arquivosInstrucoes[i] << endl;
            continue;
        }

        // Gera um quantum aleatório para o PCB
        int quantumRandom = distrib(gen);

        // Define o quantum com base no número de instruções e endereço base
        int tempoEstimado = quantidadeInstrucoes;

        // Configurando prioridade aleatória (POLÍTICA: Prioridade)
        int prioridade = prioridades[i];

        // Cria o PCB associado à faixa de memória de instruções
        PCB *novoPCB = new PCB(i + 1, quantumRandom, regs, enderecoAtual, enderecoAtual + quantidadeInstrucoes, tempoEstimado, prioridade);

        // Configura o PC inicial do processo
        novoPCB->PC = enderecoAtual;

        pcbs.push_back(novoPCB);

        // Atualiza o endereço base para o próximo conjunto de instruções
        enderecoAtual += quantidadeInstrucoes;
    }

    cout << endl;

    return pcbs;
}
