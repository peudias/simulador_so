#ifndef CACHE_HPP
#define CACHE_HPP

#include <unordered_map>
#include <list>
#include "Instruction.hpp"

class Cache
{
private:
    struct CacheEntry
    {
        Instruction instrucao;
        int resultado;
    };

    int capacidade;
    std::unordered_map<Instruction, std::list<CacheEntry>::iterator> mapaCache;
    std::list<CacheEntry> listaLRU; // Lista ordenada pela ordem de uso

public:
    explicit Cache(int capacidadeMax);

    bool contains(const Instruction &instrucao);
    int getResult(const Instruction &instrucao);
    void insert(const Instruction &instrucao, int resultado);
};

#endif // CACHE_HPP
