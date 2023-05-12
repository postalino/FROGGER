#ifndef FIUME_H
#define FIUME_H

#include "Frogger.h"
void inizializza_tronchi();
void set_tronco(int corsia, int verso, int dimensione_tronco, int traslazione_iniziale);
void* gestione_tronco(void* tronco);
void stampa_tronchi();
int tronco_taxi(int corsia, int spostamento);
#endif