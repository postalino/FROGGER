#ifndef STRADA_H
#define STRADA_H

#include "Frogger.h"

#define N_VEICOLI 6

#define STRADA_1 25
#define STRADA_2 29
#define STRADA_3 33

//prototipo funzione per generare i 6 diversi processi dei 6 veicoli
void genera_processi_veicoli(int[N_VEICOLI][2],pid_t[N_VEICOLI]);

#endif