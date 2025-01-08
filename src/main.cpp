#include "includes/Bootloader.hpp"

int main()
{
    vector<Core> cores;
    Disco disco;
    Escalonador escalonador;
    Perifericos periferico;
    RAM ram;
    Registers regs;

    Bootloader::loadConfigBootloader("data/configBootloader.txt");
    Bootloader::inicializarSistema(cores, disco, escalonador, periferico, ram, regs);

    return 0;
}