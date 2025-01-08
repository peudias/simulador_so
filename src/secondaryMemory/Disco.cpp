#include "../includes/Disco.hpp"

Disco::Disco() : memoria(LINHAS, vector<pair<int, bool>>(COLUNAS, {0, false})) {}

void Disco::write(int valor)
{
    lock_guard<mutex> lock(mtx); // Bloqueia o mutex enquanto a função é executada
    bool inserido = false;

    for (int j = 0; j < COLUNAS && !inserido; ++j)
    {
        for (int i = 0; i < LINHAS; ++i)
        {
            if (!memoria[i][j].second)
            {                                  // Verifica se a célula está livre
                memoria[i][j] = {valor, true}; // Insere o valor
                // cout << "Valor " << valor << " inserido em [" << i << "][" << j << "]" << endl;
                inserido = true;
                return;
            }
        }
    }

    if (!inserido)
    {
        cerr << "Erro: Disco cheio!" << endl;
    }
}

void Disco::display(ofstream &outfile) const
{
    for (int i = 0; i < LINHAS; ++i)
    {
        for (int j = 0; j < COLUNAS; ++j)
        {
            int valor = memoria[i][j].first;
            bool preenchido = memoria[i][j].second;

            outfile << (preenchido ? valor : 0) << " ";
        }
        outfile << endl;
    }
}

void Disco::setRegistersFromFile(Registers &regs, const string &regsFilename)
{
    ifstream regsFile(regsFilename);
    if (!regsFile.is_open())
    {
        cerr << "Erro ao abrir o arquivo de registradores: " << regsFilename << endl;
        return;
    }

    string line;
    while (getline(regsFile, line))
    {
        int regNum, regValue;
        char virgula;
        stringstream ss(line);

        ss >> regNum >> virgula >> regValue;
        regs.set(regNum, regValue);
    }
    regsFile.close();
}

int Disco::loadInstructionsFromFile(RAM &ram, const string &instrFilename, int enderecoBase)
{
    ifstream file(instrFilename);
    if (!file.is_open())
    {
        cerr << "Erro ao abrir o arquivo de instruções: " << instrFilename << endl;
        return -1;
    }

    string line;
    int enderecoAtual = enderecoBase; // Use o enderecoBase como ponto inicial

    while (getline(file, line))
    {
        string opcodeStr;
        int reg1, reg2, reg3;
        char virgula;

        stringstream ss(line);

        getline(ss, opcodeStr, ',');

        opcodeStr.erase(remove_if(opcodeStr.begin(), opcodeStr.end(), ::isspace), opcodeStr.end());

        ss >> reg1 >> virgula >> reg2 >> virgula >> reg3;

        Opcode opcode;
        if (opcodeStr == "ADD")
            opcode = ADD;
        else if (opcodeStr == "SUB")
            opcode = SUB;
        else if (opcodeStr == "AND")
            opcode = AND;
        else if (opcodeStr == "OR")
            opcode = OR;
        else if (opcodeStr == "STORE")
            opcode = STORE;
        else if (opcodeStr == "LOAD")
            opcode = LOAD;
        else if (opcodeStr == "ENQ")
            opcode = ENQ;
        else if (opcodeStr == "IF_igual")
            opcode = IF_igual;
        else
        {
            cerr << "Erro: Instrução inválida no arquivo: " << opcodeStr << endl;
            continue;
        }

        Instruction instr(opcode, reg1, reg2, reg3);

        // Verifique se o endereço está dentro dos limites da RAM
        if (enderecoAtual < ram.tamanho)
        {
            ram.instruction_memory[enderecoAtual++] = instr;
        }
        else
        {
            cerr << "Erro: memória RAM cheia, não é possível carregar mais instruções." << endl;
            break;
        }
    }
    file.close();

    // Retorne o número de instruções carregadas
    return enderecoAtual - enderecoBase;
}

vector<string> Disco::listInstructionsFile(const string &dir)
{
    vector<string> files;
    for (const auto &entry : filesystem::directory_iterator(dir))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".txt")
        {
            files.push_back(entry.path().string());
        }
    }
    sort(files.begin(), files.end());
    if (files.empty())
    {
        cerr << "Erro: Nenhum arquivo de instruções encontrado no diretório: " << dir << endl;
    }
    return files;
}