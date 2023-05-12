#ifndef STAMPA_H
#define STAMPA_H

#include "Frogger.h" 

//funzioni grafiche
int calcola_background(int x , int y);
void print_sprite(int x,int y, const char *sprite[]);
void print_barra_tempo();
void print_sprite_menu(WINDOW *name_win, int x, int y, const char *sprite[]);
void mappa_frogger();

//funzioni logiche
int abilita_movimento_confini_mappa(int direzione);

//creazione finestra di gioco
WINDOW* crea_finestra();

//play
void play_frogger();
#endif