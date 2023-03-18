#include "Frogger.h"

int main()
{
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
    play_frogger(fd_time[0],fd_rana[0]);

    delwin(win_mappa);
    endwin();
}