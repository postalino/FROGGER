#ifndef STAMPA_H
#define STAMPA_H

#include "Frogger.h" 

//funzioni grafiche
int calcola_background(int x , int y);
void print_barra_tempo(int fd_time);
void print_sprite(int x,int y, const char *sprite[]);
void mappa_frogger(int fd_time);

//funzioni logiche
int abilita_movimento_confini_mappa(oggetto_rana npc, int direzione);

//creazione finestra di gioco
WINDOW* crea_finestra();

//play
void play_frogger(int fd_time,int fd_rana, int fd_tronchi[N_CORSIE_FIUME][2],int fd_veicoli[N_VEICOLI][2], int fd_proiettile_alleati[N_MAX_P][2], int fd_sparo);
#endif