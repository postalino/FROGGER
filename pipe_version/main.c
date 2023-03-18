#include "Frogger.h"

int main()
{
    pid_t p_rana, p_tronco[N_CORSIE_FIUME],p_veicoli[N_VEICOLI], p_time;
    int fd_veicolo [N_VEICOLI][2];
    int fd_rana[2];
    int fd_tronchi[N_CORSIE_FIUME][2];
    int fd_time[2];

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


    //crea la finestra e attiva/disattiva i comandi richiesti
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, true);
    start_color();
    curs_set(0);

    //inizio gioco
    play_frogger(fd_time[0]);


    //CHECK_PIPE(fd_rana);//verifica se la pipe e' stata creata correttamente
    //CHECK_PID(p_rana);//creo processo figlio rana


   
    endwin();
    delwin(win_mappa);
}