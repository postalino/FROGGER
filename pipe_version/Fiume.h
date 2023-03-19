#ifndef FIUME_H
#define FIUME_H

#include "Frogger.h"

//funzioni
void inizializza_tronco(oggetto_tronco *npc, int corsia, int verso, int dimensione_tronco);
void stampa_tronchi (oggetto_tronco tronchi[N_CORSIE_FIUME]);
void generazione_processi_tronco(int fd_tronchi[N_CORSIE_FIUME][2], pid_t processo_tronco[N_CORSIE_FIUME], oggetto_tronco npc[N_CORSIE_FIUME]);
void lettura_pipe_tronchi(oggetto_tronco tronchi[N_CORSIE_FIUME], int fd_tronco[N_CORSIE_FIUME][2]);
void gestione_tronco(int fd_tronco, oggetto_tronco npc_tronco);

#endif