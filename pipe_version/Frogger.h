#ifndef FROGGER_H
#define FROGGER_H 

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <fcntl.h>

//sprite personaggi gioco
static const char *FROGGER[] = 
{
        "  @...@  ",
        " (\\---/) ",
        "(.>___<.)",
        "^^^   ^^^"
};

static const char *CAR_RIGHT[] = 
{
        "    _____     ",
        " __/__|__\\___ ",
        "|__\\__|__/___|",
        "  ()      ()  "
};

static const char *CAR_LEFT[] = 
{
        "     _____    ",
        " ___/__|__\\__ ",
        "|___\\__|__/__|",
        "  ()      ()  "
};

static const char *BUS_RIGHT[] = 
{
        " __________________________ ",
        "|_|   _   ___.___   _    | |",
        "|____|_|___________|_|___|_|",
        "   ()) ())       ()) ())    "
};

static const char *BUS_LEFT[] = 
{
        " __________________________ ",
        "| |    _   ___.___   _   |_|",
        "|_|___|_|_________|_|_|____|",
        "    (() (()       (() (()   "
};

static const char *TRUCK_RIGHT[] = 
{
        "               ||_||__    ",
        "|______________||   |_|___",
        "|_______________|_________\\",
        "   ()) ())         ()) ()) "
};

static const char *TRUCK_LEFT[] = 
{
        "     __||_||               ",
        " ___|_|   ||______________|",
        "/_________|_______________|",
        "  (() (()        (() (()   "
};

static const char *TRUNK_X2[] = 
{
        "  ______________  ",
        " / \\  ____      \\",
        "| @ | ____@      |",
        " \\_/____________/"
};

static const char *TRUNK_X3[] = 
{
        "  _______________________  ",
        " / \\            ____     \\ ",
        "| @ | ____@               |",
        " \\_/_____________________/ "
};

//macro calcolo dimensione sprite
#define DIM_FROGGER sizeof(FROGGER)/8 //dimensione sprite CAR_LEFT e CAR_RIGHT
#define DIM_CAR sizeof(CAR_RIGHT)/8 //dimensione sprite CAR_LEFT e CAR_RIGHT
#define DIM_BUS sizeof(BUS_RIGHT)/8 //dimensione sprite BUS_LEFT e BUS_RIGHT
#define DIM_TRUCK sizeof(TRUCK_RIGHT)/8 //dimensione sprite TRUCK_LEFT e TRUCK_RIGHT
#define DIM_TRUNK_X2 sizeof(TRUNK_X2)/8 //dimensione sprite TRUNK da 2 spazi
#define DIM_TRUNK_X3 sizeof(TRUNK_X3)/8 //dimensione sprite TRUNK da tre spazi 

//ID sprite
#define ID_FROGGER 0
#define ID_CAR_RIGHT 1
#define ID_BUS_RIGHT 2
#define ID_TRUCK_RIGHT 3
#define ID_CAR_LEFT 4
#define ID_BUS_LEFT 5
#define ID_TRUCK_LEFT 6
#define ID_TRUNK_X2 7
#define ID_TRUNK_X3 8

//dimensioni mappa
#define MAXX 99
#define MAXY 40

//dimesioni rana
#define H_FROGGER 4
#define L_FROGGER 9

//lunghezza tronco
#define L_TRUNK_X2 18
#define L_TRUNK_X3 27

//numero corsie scenario
#define N_CORSIE_STRADA 3
#define N_CORSIE_FIUME 3

//numero tane
#define N_TANE 5

//confini scenari gioco
#define MAX_PRATO 4
#define MIN_FIUME 8 
#define MAX_FIUME 19
#define MIN_STRADA 24
#define MAX_STRADA 35

//macro per la creazione di un processo o una pipe
#define CHECK_PIPE(x) {if(pipe(x)== -1) {perror("pipe call"); _exit(1);} }
#define CHECK_PID(x) {x= fork(); if(x < 0) {perror("fork call");_exit(2);}}

typedef struct tronco
{
    int x;
    int y;
    int id_sprite;
    int corsia;
    int verso;
    //to be continued...
}oggetto_tronco;

typedef struct veicolo
{
    int x;
    int y;
    int id_sprite;
    //to be continued...
}oggetto_veicolo;

typedef struct rana
{
    int x;
    int y;
    int id_sprite;
    _Bool sentinella_trasporto;
    int verso; 
}oggetto_rana;

typedef struct proiettile
{
    int x;
    int y;
    char simbolo;
    int verso_proiettile;
}oggetto_proiettile;

#include "Collisioni.h"
#include "Fiume.h"
#include "NPC.h"
#include "Stampa.h"
#include "Strada.h"

#endif





