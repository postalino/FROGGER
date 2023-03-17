#include "Frogger.h"

int main()
{
    pid_t p_rana, p_tronco[N_CORSIE_FIUME],p_veicoli[N_VEICOLI];
    int fd_veicolo [N_VEICOLI][2];
    int fd_rana[2];
    int fd_tronchi[N_CORSIE_FIUME][2];

    genera_processi_veicoli(fd_veicolo,p_veicoli);

    //crea la finestra e attiva/disattiva i comandi richiesti
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, true);
    start_color();
    curs_set(0);
    play_frogger();
    //CHECK_PIPE(fd_rana);//verifica se la pipe e' stata creata correttamente
    //CHECK_PID(p_rana);//creo processo figlio rana


    endwin();
}