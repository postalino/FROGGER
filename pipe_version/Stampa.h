#ifndef STAMPA_H
#define STAMPA_H

#include "Frogger.h"

//funzioni
int calcola_background(int x , int y);
int abilita_movimento_confini_tane(oggetto_rana npc, int direzione);
void print_sprite(int x,int y,const char *sprite[], int size);
void mappa_frogger(WINDOW *win_mappa);
WINDOW* crea_finestra();
void play_frogger();

#endif