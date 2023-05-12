#include "Frogger.h"

#define INVIO 10
#define N_SCELTE_START 4
#define N_SCELTE_END 2

volatile int running;			// Flag di programma non terminato
/* Mutex e semaforo per la sincronizzazione tra i thread */

int main()
{
    pthread_t tRana, tTronchi[N_CORSIE_FIUME];		// Pid del figlio 'rana'
    
    //crea la finestra e attiva/disattiva i comandi richiesti
    initscr();
    noecho();
    cbreak();
    start_color();
    curs_set(0);

    win_mappa = crea_finestra();
    //running = TRUE;		// Gioco non ancora finito

    pthread_mutex_init (&semCurses, NULL); // Mutex di ncurses
    
    /*    PLAYER    */
    CREATE_THREAD(tRana, wasd_rana, NULL);

    /*    TRONCHI    */
    inizializza_tronchi();
    for (size_t i = 0; i < N_CORSIE_FIUME; i++)
    {
        CREATE_THREAD(tTronchi[i], gestione_tronco,(void*)&tronchi[i]);
    }
    

    play_frogger();

    /* Attendo la terminazione del thread */
    pthread_join (tRana, NULL);		

    /* Elimino il mutex ed il semaforo */
    pthread_mutex_destroy (&semCurses);				

    endwin();		// Torno in modalita' normale

    return 0;		// Termine del programma
}
