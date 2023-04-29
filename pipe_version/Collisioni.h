#ifndef COLLISIONI_H
#define COLLISIONI_H

#include "Frogger.h"

void tempoDiGioco(int fd);
void inizializza_proiettili(int fd_proiettile_alleati[N_MAX_P][2]);
void gestione_processi_proiettili_alleati(pid_t processi[N_MAX_P], int fd_alleati[N_MAX_P][2], oggetto_rana player);
void gestione_proiettile(int fd_alleati);
void lettura_proiettili_alleati(int fd_alleati[N_MAX_P][2]);
void stampa_proiettili();
#endif