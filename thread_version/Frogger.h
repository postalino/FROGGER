#ifndef FROGGER_H
#define FROGGER_H 

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <ncurses.h>
#include <stdbool.h>
#include <time.h>

#include <pthread.h>
#include <semaphore.h>

//sprite personaggi gioco
static const char *FROGGER[] = 
{
        "  @...@  ",
        " (\\---/) ",
        "(.>___<.)",
        "^^^   ^^^",
        NULL
};

static const char *CAR_RIGHT[] = 
{
        "    _____     ",
        " __/__|__\\___ ",
        "|__\\__|__/___|",
        "  ()      ()  ",
        NULL
};

static const char *CAR_LEFT[] = 
{
        "     _____    ",
        " ___/__|__\\__ ",
        "|___\\__|__/__|",
        "  ()      ()  ",
        NULL
};

static const char *BUS_RIGHT[] = 
{
        " __________________________ ",
        "|_|   _   ___.___   _    | |",
        "|____|_|___________|_|___|_|",
        "   ()) ())       ()) ())    ",
        NULL
};

static const char *BUS_LEFT[] = 
{
        " __________________________ ",
        "| |    _   ___.___   _   |_|",
        "|_|___|_|_________|_|_|____|",
        "    (() (()       (() (()   ",
        NULL
};

static const char *TRUCK_RIGHT[] = 
{
        "               ||_||__    ",
        "|______________||   |_|___",
        "|_______________|_________\\",
        "   ()) ())         ()) ()) ",
        NULL
};

static const char *TRUCK_LEFT[] = 
{
        "     __||_||               ",
        " ___|_|   ||______________|",
        "/_________|_______________|",
        "  (() (()        (() (()   ",
        NULL
};

static const char *TRUNK_X2[] = 
{
        "  ______________  ",
        " / \\  ____      \\",
        "| @ | ____@      |",
        " \\_/____________/",
        NULL
};

static const char *TRUNK_X3[] = 
{
        "  _______________________  ",
        " / \\            ____     \\ ",
        "| @ | ____@               |",
        " \\_/_____________________/ ",
        NULL
};

static const char *TANA[] = 
{
        " /\\  /\\  ",
        "/  \\/  \\",
        "\\      /",
        " \\____/  ",
        NULL
};

static const char *WIN[]=
{
 ".----------------.  .----------------.  .----------------.   .----------------.  .----------------.  .-----------------.",
"| .--------------. || .--------------. || .--------------. | | .--------------. || .--------------. || .--------------. |",
"| |  ____  ____  | || |     ____     | || | _____  _____ | | | | _____  _____ | || |     _____    | || | ____  _____  | |",
"| | |_  _||_  _| | || |   .'    `.   | || ||_   _||_   _|| | | ||_   _||_   _|| || |    |_   _|   | || ||_   \\|_   _| | |",
"| |   \\ \\  / /   | || |  /  .--.  \\  | || |  | |    | |  | | | |  | | /\\ | |  | || |      | |     | || |  |   \\ | |   | |",
"| |    \\ \\/ /    | || |  | |    | |  | || |  | '    ' |  | | | |  | |/  \\| |  | || |      | |     | || |  | |\\ \\| |   | |",
"| |    _|  |_    | || |  \\  `--'  /  | || |   \\ `--' /   | | | |  |   /\\   |  | || |     _| |_    | || | _| |_\\   |_  | |",
"| |   |______|   | || |   `.____.'   | || |    `.__.'    | | | |  |__/  \\__|  | || |    |_____|   | || ||_____|\\____| | |",
"| |              | || |              | || |              | | | |              | || |              | || |              | |",
"| '--------------' || '--------------' || '--------------' | | '--------------' || '--------------' || '--------------' |",
" '----------------'  '----------------'  '----------------'   '----------------'  '----------------'  '----------------' ",
NULL
};

static const char *LOSE[]=
{
" .----------------.  .----------------.  .----------------.   .----------------.  .----------------.  .----------------.  .----------------. ",
"| .--------------. || .--------------. || .--------------. | | .--------------. || .--------------. || .--------------. || .--------------. |",
"| |  ____  ____  | || |     ____     | || | _____  _____ | | | |   _____      | || |     ____     | || |    _______   | || |  _________   | |",
"| | |_  _||_  _| | || |   .'    `.   | || ||_   _||_   _|| | | |  |_   _|     | || |   .'    `.   | || |   /  ___  |  | || | |_   ___  |  | |",
"| |   \\ \\  / /   | || |  /  .--.  \\  | || |  | |    | |  | | | |    | |       | || |  /  .--.  \\  | || |  |  (__ \\_|  | || |   | |_  \\_|  | |",
"| |    \\ \\/ /    | || |  | |    | |  | || |  | '    ' |  | | | |    | |   _   | || |  | |    | |  | || |   '.___`-.   | || |   |  _|  _   | |",
"| |    _|  |_    | || |  \\  `--'  /  | || |   \\ `--' /   | | | |   _| |__/ |  | || |  \\  `--'  /  | || |  |`\\____) |  | || |  _| |___/ |  | |",
"| |   |______|   | || |   `.____.'   | || |    `.__.'    | | | |  |________|  | || |   `.____.'   | || |  |_______.'  | || | |_________|  | |",
"| |              | || |              | || |              | | | |              | || |              | || |              | || |              | |",
"| '--------------' || '--------------' || '--------------' | | '--------------' || '--------------' || '--------------' || '--------------' |",
" '----------------'  '----------------'  '----------------'   '----------------'  '----------------'  '----------------'  '----------------' ",
NULL
};

static const char* R_LEFT_LOSE[] =
{
"                  ",   
"  __   ___.--'_`. ",   
" ( _`.'. -   'o` )",   
" _\\.'_'      _.-' ",    
"( \\`. )    //\\`   ",     
" \\_`-'`---'\\__,  ",     
"  \\`        `-\\   ",     
"   `              ",
NULL      
};

static const char* R_RIGHT_LOSE[] = 
{
" .'_`--.___   __  ",
"( 'o`   - .`.'_ ) ",
" `-._      `_`./_ ",
"   '/\\    ( .'/ )",
"  ,__//`---'`-'_/ ",
"   /-'        '/  ",
"              '   ",
        NULL
};

static const char* R_LR_WIN[] =
{
"         o  o   o  o       ",
"         |\\/ \\^/ \\/|       ",
"         |,-------.|       ",
"       ,-.(|)   (|),-.     ",
"       \\_*._ ' '_.* _/     ",
"        /`-.`--' .-'\\      ",
"   ,--./    `---'    \\,--. ",
"   \\   |(  )     (  )|   / ",
"    \\  | ||       || |  /  ",
"     \\ | /|\\     /|\\ | /   ",
"     /  \\-._     _,-/  \\   ",
"    //| \\  `---'  // |\\  ",
"   /,-.,-.\\       /,-.,-.\\ ",
"  o   o   o      o   o    o",
NULL
};

static const char* FROG_MENU[] =
{
" .----------------.  .----------------.  .----------------.  .----------------.  .----------------.  .----------------.  .----------------. ",
"| .--------------. || .--------------. || .--------------. || .--------------. || .--------------. || .--------------. || .--------------. |",
"| |  _________   | || |  _______     | || |     ____     | || |    ______    | || |    ______    | || |  _________   | || |  _______     | |",
"| | |_   ___  |  | || | |_   __ \\    | || |   .'    `.   | || |  .' ___  |   | || |  .' ___  |   | || | |_   ___  |  | || | |_   __ \\    | |",
"| |   | |_  \\_|  | || |   | |__) |   | || |  /  .--.  \\  | || | / .'   \\_|   | || | / .'   \\_|   | || |   | |_  \\_|  | || |   | |__) |   | |",
"| |   |  _|      | || |   |  __ /    | || |  | |    | |  | || | | |    ____  | || | | |    ____  | || |   |  _|  _   | || |   |  __ /    | |",
"| |  _| |_       | || |  _| |  \\ \\_  | || |  \\  `--'  /  | || | \\ `.___]  _| | || | \\ `.___]  _| | || |  _| |___/ |  | || |  _| |  \\ \\_  | |",
"| | |_____|      | || | |____| |___| | || |   `.____.'   | || |  `._____.'   | || |  `._____.'   | || | |_________|  | || | |____| |___| | |",
"| |              | || |              | || |              | || |              | || |              | || |              | || |              | |",
"| '--------------' || '--------------' || '--------------' || '--------------' || '--------------' || '--------------' || '--------------' |",
" '----------------'  '----------------'  '----------------'  '----------------'  '----------------'  '----------------'  '----------------' ",
NULL
};

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
#define ID_TANA 9
#define ID_ENEMY 10

//dimensioni mappa totale
#define MAXX 159
#define MAXY 42

//dimensione mappa visiva
/*se utili si aggiungono*/

//posizione default rana
#define X_START 30 + L_FROGGER*5 
#define Y_START (MAXY -2) - H_FROGGER

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
#define CREATE_THREAD(thread,funzione, parametro) {if (pthread_create (&thread, NULL, funzione, parametro)){endwin();printf ("Non riesco a creare il thread del ladro\n");exit;}}
#define CHECK_WINDOW(x) {if (x == NULL) { fprintf(stderr, "Errore creazione finestra!\n");exit(EXIT_FAILURE);}} 

typedef struct tronco
{
    int x;
    int y;
    int id_sprite;
    int corsia;
    int verso;
}oggetto_tronco;

typedef struct veicolo
{
    int x;
    int y;
    int id_sprite;
    int timer;
    int verso;
}oggetto_veicolo;

typedef struct rana
{
    int x;
    int y;
    int id_sprite; 
}oggetto_rana;

typedef struct proiettile
{
    int x;
    int y;
    int verso_proiettile;
}oggetto_proiettile;

typedef struct tana
{
    int x;
    int y;
    int id_sprite;
    bool occupata;
}oggetto_tana;

//ID colori oggetti e scenari
#define PRATO 1
#define FIUME 2
#define MARCIAPIEDE 3
#define STRADA 4
#define RANA 5
#define TRONCO 6
#define T_MAX_WORD 7
#define T_MIDLE_WORD 8
#define T_MIN_WORD 9
#define T_MAX_BAR 10
#define T_MIDLE_BAR 11
#define T_MIN_BAR 12
#define TANA_OCCUPATA 13
#define VEICOLO 14
#define PLAY_AGAIN 15
#define ENEMY 16
#define PROIETTILE 17


//delay tempi
#define TIME 400000  //temporizzazione velocità tronchi
#define TIME_MAIN 100000 //temporizzazione aggiornamento mappa

// numero veicoli
#define N_VEICOLI 6

// proiettili massimi
#define N_MAX_P 15

//nemici massimi
#define N_MAX_ENEMY 3

//variabili globali
pthread_mutex_t	semCurses;	// Semaforo per l'accesso alle funzioni di ncurses
sem_t	semGioco;		// Semaforo per sincronizzare il campo con i 'giocatori'
WINDOW *win_mappa;

volatile oggetto_rana player; 
volatile oggetto_tronco tronchi[N_CORSIE_FIUME];
volatile int vite;
volatile int max_time;
volatile oggetto_tana tane[N_TANE];


#include "Collisioni.h"
#include "Fiume.h"
#include "NPC.h"
#include "Stampa.h"
#include "Strada.h"

#endif