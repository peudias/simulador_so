#include "../includes/Core.hpp"

#define OUTPUT_DIR "data/logs"

Core::Core(RAM &ram, Disco &disco, Escalonador &escalonador)
    : ram(ram), disco(disco), PC(0), Clock(0), escalonador(escalonador) {}

void Core::activate(ofstream &outfile)
{
    while (escalonador.temProcessosProntos())
    {
        auto inicioOcupado = chrono::high_resolution_clock::now();
        PCB *pcb = escalonador.obterProximoProcesso(outfile);

        outfile << "Processo " << pcb->pid
                << ": Base (Instruções) = " << pcb->enderecoBaseInstrucoes
                << ", Limite (Instruções) = " << pcb->enderecoLimiteInstrucoes
                << ", PC (Inicial) = " << pcb->PC
                << ", Quantum = " << pcb->quantumProcesso
                << endl;

        if (pcb == nullptr)
        {
            outfile << "[Núcleo " << this_thread::get_id() << "] Erro: Nenhum processo disponível para execução.\n";
            return;
        }

        // Restaurar o estado do processo
        auto pipelineState = pipeline.getPipelineState();
        pcb->restaurarEstado(pipelineState, outfile);

        pcb->atualizarEstado(EXECUCAO, outfile);
        outfile << "[Núcleo " << this_thread::get_id() << "] Iniciando execução do processo [PID: " << pcb->pid << "]\n";
        outfile << "\nANTES DA EXECUÇÃO";
        pcb->exibirPCB(outfile); // Imprime o estado inicial do PCB

        // Loop principal de execução de processo
        while (true)
        {
            // Verifica se o quantum expirou
            if (pcb->quantumExpirado())
            {
                outfile << "[Quantum Expirado] Processo " << pcb->pid << " NÃO TERMINOU. Quantum Alocado Insuficitente! Vai para o Fim da Fila!\n\n";
                pcb->salvarEstado(pipeline.getPipelineState()); // Salva o estado completo
                break;
            }

            // Valida se o PC está dentro do limite de instruções antes do fetch
            if (pcb->PC >= pcb->getLimiteInstrucoes())
            {
                pcb->PC += 1;

                int instrucoesRestantes = pcb->getLimiteInstrucoes() - pcb->PC + 1;
                if (instrucoesRestantes <= 0)
                    instrucoesRestantes = 0;

                outfile << "[Quantum] Processo " << pcb->pid
                        << " | Restante: " << pcb->quantumRestante
                        << " | Tarefas Restantes: " << instrucoesRestantes << endl;

                outfile << "\n[Núcleo " << this_thread::get_id() << "] Processo " << pcb->pid
                        << " executou todas as instruções (PC: " << pcb->PC
                        << ", Base: " << pcb->getEnderecoBaseInstrucoes()
                        << ", Limite: " << pcb->getLimiteInstrucoes() << ")\n";

                pcb->atualizarEstado(FINALIZADO, outfile);
                break;
            }

            // Busca a próxima instrução
            Instruction instr = pipeline.InstructionFetch(ram, pcb->PC, pcb);
            if (instr.op == NOP)
            {
                outfile << "[Núcleo " << this_thread::get_id() << "] Instrução inválida no PC: " << pcb->PC << ". Encerrando execução.\n";
                pcb->atualizarEstado(FINALIZADO, outfile);
                break;
            }

            // Executa a instrução
            uc.executarInstrucao(instr, pcb->registradores, ram, pcb->PC, disco, Clock, *pcb, outfile);
            // cout << "[Pipeline] Executando instrução: Opcode " << instr.op << "\n";

            // Incrementa o PC
            pcb->PC += 1; // Incremento em unidades para acompanhar a RAM

            // Decrementa o quantum
            pcb->decrementarQuantum(outfile);
        }

        // Salvar o estado do processo
        pcb->salvarEstado(pipeline.getPipelineState());
        outfile << "[Núcleo " << this_thread::get_id() << "] Encerrando a execução do processo [PID: " << pcb->pid << "].\n";
        if (pcb->quantumExpirado())
        {
            pcb->atualizarEstado(BLOQUEADO, outfile);
        }
        outfile << "\nDEPOIS DA EXECUÇÃO";
        pcb->exibirPCB(outfile); // Exibe o estado final do PCB

        // Gerenciamento de estados
        if (pcb->verificarEstado(FINALIZADO))
        {
            outfile << "[Núcleo " << this_thread::get_id() << "] Processo [PID: " << pcb->pid << "] finalizado.\n";
            outfile << "************************************************************************************************************************\n";
        }
        else if (pcb->quantumExpirado())
        {
            pcb->resetarQuantum(outfile);
            pcb->atualizarEstado(PRONTO, outfile);
            escalonador.adicionarProcesso(pcb, outfile);
            outfile << "[Núcleo " << this_thread::get_id() << "] Quantum expirado para o processo [PID: " << pcb->pid
                    << "]. Retornando à fila de prontos.\n";
        }
        else if (pcb->verificarEstado(BLOQUEADO))
        {
            outfile << "[Núcleo " << this_thread::get_id() << "] Processo [PID: " << pcb->pid << "] bloqueado.\n";
        }
        auto fimOcupado = chrono::high_resolution_clock::now();
        tempoOcupado += chrono::duration_cast<chrono::duration<double, milli>>(fimOcupado - inicioOcupado).count();
    }
    auto inicioOcioso = chrono::high_resolution_clock::now();
    this_thread::sleep_for(chrono::milliseconds(1));
    auto fimOcioso = chrono::high_resolution_clock::now();
    tempoOcioso += chrono::duration_cast<chrono::duration<double, milli>>(fimOcioso - inicioOcioso).count();
}

void Core::run()
{
    if (!filesystem::exists(OUTPUT_DIR))
    {
        filesystem::create_directory(OUTPUT_DIR);
    }

    ostringstream oss;
    oss << this_thread::get_id();
    string this_thread_id_str = oss.str();

    // Nome do output para este núcleo
    string filename = string(OUTPUT_DIR) + "/output_nucleo_" + this_thread_id_str + ".txt";
    ofstream outfile(filename, ios::out);

    if (!outfile.is_open())
    {
        cerr << "[Núcleo " << this_thread::get_id() << "] Erro ao criar arquivo de saída.\n";
        return;
    }

    outfile << "[Núcleo] ID: " << this_thread::get_id() << "\n";

    // cout << "\n" << "Executando o núcleo (Thread) com PC: " << PC << "\n\n";
    activate(outfile);
    exibirTempoCore(outfile);
    outfile.close();
}

void Core::exibirTempoCore(ofstream &outfile)
{
    outfile << fixed << setprecision(3);
    outfile << endl
            << "=============== Estatísticas do Núcleo ================" << endl;
    outfile << "Tempo ocupado: " << tempoOcupado << " ms\n";
    outfile << "Tempo ocioso: " << tempoOcioso << " ms\n";

    if (tempoOcupado + tempoOcioso > 0)
    {
        outfile << "Taxa de utilização: "
                << (tempoOcupado / (tempoOcupado + tempoOcioso)) * 100 << " %\n";
    }
    else
    {
        outfile << "Taxa de utilização: 0.000 %\n";
    }
}

void Core::validateMemoryAccess(PCB *processo, int endereco, ofstream &outfile)
{
    if (!processo->verificarAcessoMemoria(endereco) || ram.isReserved(endereco))
    {
        outfile << "[Erro] Acesso inválido à memória no endereço " << endereco << " pelo processo " << processo->pid << "\n";
        processo->atualizarEstado(BLOQUEADO, outfile); // Bloqueia o processo caso o acesso seja inválido
    }
}

void Core::gerenciarRecursos(PCB *processo, ofstream &outfile)
{
    // Simula uma verificação de recursos
    string recursoNecessario = "teclado"; // Exemplo de recurso
    if (!processo->verificarRecurso(recursoNecessario))
    {
        processo->associarRecurso(recursoNecessario, true); // Aloca o recurso
    }
    else
    {
        processo->atualizarEstado(BLOQUEADO, outfile); // Bloqueia o processo caso o recurso esteja indisponível
    }
}
