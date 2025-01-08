#include "../includes/UnidadeControle.hpp"

void UnidadeControle::executarInstrucao(const Instruction &instr, Registers &regs, RAM &ram, int &PC, Disco &disco, int &Clock, PCB &pcb, ofstream &outfile)
{
    vector<int> pipelineState;                                    // Pipeline inicializado para restaurar
    pcb.restaurarEstado(pipelineState, outfile);                  // Restaura o estado completo do processo
    PC = pcb.PC;                                                  // Atualiza o PC do processo
    pipeline.PipelineProcess(instr, regs, ram, PC, disco, Clock); // Executa o pipeline do processo
    pcb.PC = PC;                                                  // Atualiza o estado do PCB após a execução
    pcb.salvarEstado(pipeline.getPipelineState());                // Atualiza o PCB com o novo estado
}