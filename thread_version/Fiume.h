#ifndef FIUME_H
#define FIUME_H

#include "Frogger.h"

/*  TRONCO  */
void inizializza_tronchi();
void set_tronco(int corsia, int verso, int dimensione_tronco, int traslazione_iniziale);
void* gestione_tronco(void* tronco);
void stampa_tronchi();

/*  RANA SU TRONCO  */
int tronco_taxi(int corsia, int spostamento);

/*  TANE    */
int check_tana(int x, int y);
int tana_occupata();
void fuori_area_tane ();
void inizializza_posizione_tane();
void print_tane_occupate();
void collisioni_tane_occupate();

/*  VITTORIA    */
int vittoria();

#endif