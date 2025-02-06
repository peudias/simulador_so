#include "../includes/Cache.hpp"

Cache::Cache(int capacidadeMax) : capacidade(capacidadeMax) {}

bool Cache::contains(const Instruction &instrucao)
{
    return mapaCache.find(instrucao) != mapaCache.end();
}

int Cache::getResult(const Instruction &instrucao)
{
    auto it = mapaCache.find(instrucao);
    if (it == mapaCache.end())
    {
        throw std::runtime_error("Erro: Instrução não encontrada na Cache.");
    }

    // Move a instrução para o topo da lista (LRU)
    listaLRU.splice(listaLRU.begin(), listaLRU, it->second);
    return it->second->resultado;
}

void Cache::insert(const Instruction &instrucao, int resultado)
{
    // Se a instrução já existe, move para o topo
    if (contains(instrucao))
    {
        mapaCache[instrucao]->resultado = resultado;
        listaLRU.splice(listaLRU.begin(), listaLRU, mapaCache[instrucao]);
        return;
    }

    // Se a Cache está cheia, remove o menos recentemente usado
    if (listaLRU.size() >= capacidade)
    {
        mapaCache.erase(listaLRU.back().instrucao);
        listaLRU.pop_back();
    }

    // Insere a nova instrução no topo da lista
    listaLRU.push_front({instrucao, resultado});
    mapaCache[instrucao] = listaLRU.begin();
}
