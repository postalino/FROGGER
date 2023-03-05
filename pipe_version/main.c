#include "Frogger.h"

int main()
{
    pid_t p_rana, p_tronco[N_CORSIE_FIUME];
    int fd_rana[2];
    int fd_tronchi[N_CORSIE_FIUME][2];

    //crea la finestra e attiva/disattiva i comandi richiesti
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, true);
    start_color();
    curs_set(0);

    //CHECK_PIPE(fd_rana);//verifica se la pipe e' stata creata correttamente
    //CHECK_PID(p_rana);//creo processo figlio rana


    endwin();
}