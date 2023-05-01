#ifndef COLLISIONI_H
#define COLLISIONI_H

#include "Frogger.h"

//tempo di gioco
void tempoDiGioco(int fd);

//collisioni veicolo
int collisioni_rana_veicoli(oggetto_rana rana,oggetto_veicolo veicolo [N_VEICOLI]);

//collisione player con rana nemica
void collisione_player_enemy(oggetto_rana *player, int* vite);

//creazione e gestione oggetto proiettile
void inizializza_proiettili(int fd_proiettile_alleati[N_MAX_P][2], pid_t processi[N_MAX_P], oggetto_proiettile proiettili[N_MAX_P]);
void gestione_processi_proiettili(oggetto_rana player);
void gestione_proiettile(int fd_alleati);
void lettura_proiettili(int fd_alleati[N_MAX_P][2], oggetto_proiettile proiettili[N_MAX_P]);
void stampa_proiettili();

//collisioni proiettili
void collisioni_proiettili_bordi();
void collisioni_proiettiliA_proiettiliN();
void collisioni_proiettile_enemy();
void collisioni_proiettiliN_macchine();
void collisione_player_proiettileN(oggetto_rana *player, int *vite);
void collisioni_game(oggetto_rana *player, int* vite);
#endif