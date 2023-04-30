#ifndef NPC_H
#define NPC_H

#include "Frogger.h"

//funzioni player
void wasd_rana(int fd_rana, int fd_sparo);
void update_position_frogger(int spostamento, oggetto_rana* npc);

//funzioni enemy
void inizializza_enemy(int fd_enemy[N_MAX_ENEMY][2], pid_t processi_enemy[N_MAX_ENEMY]);
void crea_processo_enemy(pid_t processi_enemy[N_MAX_P], int fd_rana_enemy[N_MAX_P][2]);
void gestione_enemy(int fd_rana_enemy);
void stampa_enemy();
void lettura_enemy(int fd_enemy[N_MAX_ENEMY][2]);
void genera_enemy();

#endif