#include "Frogger.h"

int main()
{
    //inizializzo il seme della rand con time
    srand(time(NULL));

    int verso, dimensione_tronco;
    int traslazioni[N_CORSIE_FIUME];

    //calcolo randomicamente una traslazione iniziale per avere movimenti diversi dei tronchi
    for (int i = 0; i < N_CORSIE_FIUME; i++) {
        traslazioni[i] = (L_FROGGER*(rand()%4));  // genero l'incremento casuale [set casuale-->(0,9,18,27)]
        for (int j = 0; j < i; j++) {
            if (traslazioni[j] == traslazioni[i]) {  // controllo se l'incremento è gia stato generato
                i--;  // se sì, decrementa i per ripetere il ciclo e generarne un'altro
                break;
            }
        }
    }
    //inizializzo le strutture tronchi per avere la stessa coppia sia nel padre sia nel figlio
    for (int i = 0; i < N_CORSIE_FIUME; i++)
    {
        //determino randomicamente il verso del tronco ( 0 -. destra | 1 -. sinistra)
        verso = rand()%2;
        //determino randomicamente la dimensione del tronco (0 -. x2 | 1 -. x3)
        dimensione_tronco = rand()%2;
        //inizializza la struttura specifica
        inizializza_tronco(&tronchi[i], i, verso, dimensione_tronco, traslazioni[i]);
    }

    //crea la finestra e attiva/disattiva i comandi richiesti
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, true);
    start_color();
    curs_set(0);

    pid_t p_rana, p_tronco[N_CORSIE_FIUME],p_veicoli[N_VEICOLI], p_time;
    int fd_veicolo [N_VEICOLI][2];
    int fd_rana[2];
    int fd_tronchi[N_CORSIE_FIUME][2];
    int fd_time[2];

    // creazione finestra principale centrata
    win_mappa = crea_finestra();
    CHECK_WINDOW(win_mappa); //verifica se a finestra e' stata creata correttamente

    genera_processi_veicoli(fd_veicolo,p_veicoli);

    generazione_processi_tronco(fd_tronchi,p_tronco,tronchi);

    CHECK_PIPE(fd_time);    //verifica se la pipe e' stata creata correttamente
    fcntl(fd_time[0],F_SETFL, O_NONBLOCK); //imposto la pipe come non bloccante
    CHECK_PID(p_time);  //creo processo figlio tempo di gioco
    
    switch (p_time)
    {
    case 0:
        /* processo figlio */
        close(fd_time[0]);  //chiusura file descriptor in lettura
        tempoDiGioco(fd_time[1]);
        break;
    
    default:
        /* processo padre */
        close(fd_time[1]); //chiusura file descriptor in scrittura
        break;
    }

    CHECK_PIPE(fd_rana);    //verifica se la pipe e' stata creata correttamente
    fcntl(fd_rana[0],F_SETFL, O_NONBLOCK); //imposto la pipe come non bloccante
    CHECK_PID(p_rana);  //creo processo figlio rana

    switch (p_rana)
    {
    case 0:
        /* processo figlio */
        close(fd_rana[0]);  //chiusura file descriptor in lettura
        wasd_rana(fd_rana[1]);
        break;
    
    default:
        /* processo padre */
        close(fd_rana[1]); //chiusura file descriptor in scrittura
        break;
    }

    //inizio gioco
    play_frogger(fd_time[0],fd_rana[0], fd_tronchi);

    delwin(win_mappa);
    endwin();
}