#ifndef NPC_H
#define NPC_H

#include "Frogger.h"

//funzioni player
void* wasd_rana();

void genera_enemy();

void stampa_enemy();

void inizializza_enemy();

void* gestione_enemy(void* enemy);

void crea_proiettile_enemy(oggetto_rana *player);

#endif