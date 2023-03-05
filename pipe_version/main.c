#include "Frogger.h"

void frogger(int fd_rana, int fd_tronco[N_CORSIE_FIUME][2], int fd_trasporto_rana,int fd_veicolo[N_VEICOLI][2], int fd_proiettili_alleati [MAX_PROIETTILI][2]);

int main()
{
    
    pid_t p_rana, p_tronco[N_CORSIE_FIUME], p_time,p_veicoli[N_VEICOLI];
    int fd_rana[2];
    int fd_tronchi[N_CORSIE_FIUME][2];
    int fd_trasporto_rana[2];
    int fd_veicolo [N_VEICOLI][2];
    int fd_proiettili_alleati [MAX_PROIETTILI][2];

    //crea la finestra e attiva/disattiva i comandi richiesti
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, true);
    start_color();
    curs_set(0);

    for (int i = 0; i < N_CORSIE_FIUME; i++)
    {
        CHECK_PIPE(fd_tronchi[i]);//verifica se la pipe e' stata creata correttamente
        CHECK_PID(p_tronco[i]);//creo processo figlio tronco
        if(!p_tronco[i])
        {  
            close(fd_tronchi[i][0]);
            gestione_tronco(fd_tronchi[i][1], i);
        }
    }

    genera_processi_veicoli(fd_veicolo,p_veicoli);


    for(int i=0; i<N_CORSIE_FIUME;i++)
        kill(p_tronco[i],1);
    for(int i=0; i<N_VEICOLI;i++)
        kill(p_veicoli[i],1);
    
    endwin();
}