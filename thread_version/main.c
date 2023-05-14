#include "Frogger.h"

int main()
{
    pthread_t tRana, tTronchi[N_CORSIE_FIUME],tTempo,tVeicoli[N_VEICOLI],tProiettili_A[N_MAX_P],tEnemy[N_MAX_ENEMY];		// Pid del figlio 'rana'
    
    //crea la finestra e attiva/disattiva i comandi richiesti
    initscr();
    noecho();
    cbreak();
    start_color();
    curs_set(0);

    win_mappa = crea_finestra();
    running = true;		// Gioco non ancora finito

    pthread_mutex_init (&semCurses, NULL); // Mutex di ncurses

    /*      VEICOLI     */
    inizializza_veicoli();
    for (size_t i = 0; i < N_VEICOLI; i++)
    {
        CREATE_THREAD(tVeicoli[i],gestione_veicolo,(void*)&veicolo[i]);
    }
    
    
    /*    PLAYER    */
    CREATE_THREAD(tRana, wasd_rana, NULL);

    /*    TRONCHI    */
    inizializza_tronchi();
    for (size_t i = 0; i < N_CORSIE_FIUME; i++)
    {
        CREATE_THREAD(tTronchi[i], gestione_tronco,(void*)&tronchi[i]);
    }

    /*  TEMPO DI GIOCO*/
    CREATE_THREAD(tRana, tempoDiGioco, NULL);

    /*  PROIETTILI  */
    inizializza_proiettili();
    for (size_t i = 0; i < N_MAX_P; i++)
    {
        CREATE_THREAD(tProiettili_A[i],gestione_proiettili_A,(void*)&proiettili_alleati[i]);
    }
    /* ENEMY */
    inizializza_enemy();
    for (size_t i = 0; i < N_MAX_ENEMY; i++)
    {
        CREATE_THREAD(tEnemy[i],gestione_enemy,(void*)&enemy[i]);
    }
    
    

    play_frogger();

    /* Attendo la terminazione del thread */
    pthread_join (tRana, NULL);
    for (size_t i = 0; i < N_VEICOLI; i++)
    {
        pthread_join (tVeicoli[i], NULL);
    }
    for (size_t i = 0; i < N_CORSIE_FIUME; i++)
    {
        pthread_join (tTronchi[i], NULL);
    }

    /* Elimino il mutex ed il semaforo */
    pthread_mutex_destroy (&semCurses);
    wclear(win_mappa);			

    endwin();		// Torno in modalita' normale

    return 0;		// Termine del programma
}
