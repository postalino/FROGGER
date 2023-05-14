#ifndef COLLISIONI_H
#define COLLISIONI_H

#include "Frogger.h"

//tempo di gioco
void* tempoDiGioco();

//collisioni veicolo
void collisioni_rana_veicoli();

//creazione e gestione oggetto proiettile
void inizializza_proiettili();
void* gestione_proiettili(void*);
void stampa_proiettili();

//ferma tutti i cicli quando il gioco termina
void game_over();

//collisione player con rana nemica
void collisione_player_enemy();

//collisioni proiettili
void collisioni_proiettili_bordi();
void collisioni_proiettili_macchine();
void collisioni_proiettiliA_proiettiliN();
void collisioni_proiettile_enemy();
void collisione_player_proiettileN();
void collisioni_game();

#endif