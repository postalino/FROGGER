#ifndef COLLISIONI_H
#define COLLISIONI_H

#include "Frogger.h"

void tempoDiGioco(int fd);
void inizializza_proiettili(int fd_proiettile_alleati[N_MAX_P][2], pid_t processi[N_MAX_P]);
void gestione_processi_proiettili_alleati(oggetto_rana player);
void gestione_proiettile(int fd_alleati);
int collisioni_rana_veicoli(oggetto_rana rana,oggetto_veicolo veicolo [N_VEICOLI]);
void lettura_proiettili_alleati(int fd_alleati[N_MAX_P][2]);
void stampa_proiettili();
void collisioni_proiettili_bordi();
#endif