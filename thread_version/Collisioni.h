#ifndef COLLISIONI_H
#define COLLISIONI_H

#include "Frogger.h"

void collisioni_rana_veicoli();

void* tempoDiGioco();
void inizializza_proiettili();
void game_over();

void* gestione_proiettili(void*);
void stampa_proiettili();
void collisioni_proiettili_bordi();

#endif