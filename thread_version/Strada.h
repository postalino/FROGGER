#ifndef STRADA_H
#define STRADA_H

#include "Frogger.h"


#define STRADA_1 24
#define STRADA_2 28
#define STRADA_3 32

//prototipo funzione per generare i 6 diversi processi dei 6 veicoli

void inizializza_veicoli();

void* gestione_veicolo(void* veicolo);

void stampa_veicoli();

int check_v(int strada,int id, int n_X);

#endif