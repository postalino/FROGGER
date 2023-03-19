#ifndef FIUME_H
#define FIUME_H

#include "Frogger.h"

/*  TRONCO  */
void inizializza_tronco(oggetto_tronco *npc, int corsia, int verso, int dimensione_tronco);
void generazione_processi_tronco(int fd_tronchi[N_CORSIE_FIUME][2], pid_t processo_tronco[N_CORSIE_FIUME], oggetto_tronco npc[N_CORSIE_FIUME]);
void stampa_tronchi (oggetto_tronco tronchi[N_CORSIE_FIUME]);
void lettura_pipe_tronchi(oggetto_rana *npc_rana, oggetto_tronco tronchi[N_CORSIE_FIUME], int fd_tronco[N_CORSIE_FIUME][2]);
void gestione_tronco(int fd_tronco, oggetto_tronco npc_tronco);

/*  RANA SU TRONCO  */
void tronco_taxi(oggetto_rana *npc_rana, oggetto_tronco npc_tronco, int spostamento);

/*  TANE    */
void inizializza_posizione_tane(oggetto_tana tane[N_TANE]);
void tana_occupata(oggetto_rana * player,oggetto_tana tane[N_TANE]);
void print_tane_occupate(oggetto_tana tana[N_TANE]);
int check_tana(int x, int y);

#endif