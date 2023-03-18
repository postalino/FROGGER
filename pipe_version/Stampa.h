#ifndef STAMPA_H
#define STAMPA_H

#include "Frogger.h" 

//funzioni
int calcola_background(int x , int y);
int abilita_movimento_confini_mappa(oggetto_rana npc, int direzione);
void print_sprite(int x,int y, const char *sprite[]);
void mappa_frogger(int fd_time);
WINDOW* crea_finestra();
void print_barra_tempo(int fd_time);
void play_frogger(int fd_time,int fd_rana);

#endif