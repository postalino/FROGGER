#include "NPC.h"

#define SPOSTA_SU -4
#define SPOSTA_GIU +4
#define SPOSTA_DESTRA +9
#define SPOSTA_SINISTRA -9

#define SPACE 32

void wasd_rana(int fd_rana, int fd_sparo)
{
    int direzione;
    int incremento;
    int sparato = 1;

    
    while (true)
    {
        timeout(1);
        direzione = getch();

        switch (direzione)
        {
        case KEY_UP:
            incremento = SPOSTA_SU;
            write(fd_rana, &incremento, sizeof(incremento)); // scritture posizione successive
            break;
        case KEY_DOWN:
            incremento = SPOSTA_GIU;
            write(fd_rana, &incremento, sizeof(incremento)); // scritture posizione successive
            break;
        case KEY_LEFT:
            incremento = SPOSTA_SINISTRA;
            write(fd_rana, &incremento, sizeof(incremento)); // scritture posizione successive
            break;
        case KEY_RIGHT:
            incremento = SPOSTA_DESTRA;
            write(fd_rana, &incremento, sizeof(incremento)); // scritture posizione successive
            break;
        case SPACE:
            write(fd_sparo,&sparato, sizeof(sparato));  // segnale di sparo
            break;
        }

        if (win_mappa == NULL) {
        fprintf(stderr, "Errore della finestra.\n");
        exit(EXIT_FAILURE);
        }  

    }
}

void update_position_frogger(int spostamento, oggetto_rana* npc)
{
    if(spostamento != 0)    //se presente un movimento vedo di che tipo altriementi no
    {
        if(spostamento == +4 || spostamento == -4)
            npc->y+=spostamento;
        else if(spostamento == +9 || spostamento == -9)
            npc->x+=spostamento;
    }
}

void inizializza_enemy(int fd_enemy[N_MAX_ENEMY][2])
{
    for (size_t i = 0; i < N_MAX_ENEMY; i++)
    {
        enemy[i].x = -1;
        CHECK_PIPE(fd_enemy[i]);    //verifica se la pipe e' stata creata correttamente
    }
}

void crea_processo_enemy(pid_t processi_enemy[N_MAX_P], int fd_rana_enemy[N_MAX_P][2])
{
    // Codice del processo padre(sono tutti inizializzati a zero)
    pid_t child_pid;

    for (int i = 0; i < N_MAX_P; i++)
        {
            if(processi_enemy[i] > 0)
            {
                //il figlio esiste
                child_pid = waitpid(processi_enemy[i], NULL, WNOHANG) ;
                if (child_pid > 0) {
                    //se il figlio muore inizializziamo il pid del processo a zero
                    processi_enemy[i] = 0;
                }
            }
            else{
                 //il figlio è morto o non esiste quindi chiama la funzione per ricrearlo
                CHECK_PID(processi_enemy[i]);

                if(processi_enemy[i] == 0){
                    close(fd_rana_enemy[i][0]);
                    gestione_enemy(fd_rana_enemy[i][1]);
                }
                
                close(fd_rana_enemy[i][1]);

                i = N_MAX_P; 
            }
        }
}