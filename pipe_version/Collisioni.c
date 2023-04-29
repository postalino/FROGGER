#include "Collisioni.h"

void tempoDiGioco(int fd)
{
    int clock = 0;
    int tempo = 0;
    while (true)
    {
        sleep(1);
        clock++;
        write(fd,&clock , sizeof(clock)); //scritture tempo di gioco rimanente
        tempo++;
        clock--;
    }
}

void inizializza_proiettili(int fd_proiettile_alleati[N_MAX_P][2])
{
    for (size_t i = 0; i < N_MAX_P; i++)
    {
        proiettili_alleati[i].x = -1;
        CHECK_PIPE(fd_proiettile_alleati[i]);    //verifica se la pipe e' stata creata correttamente
    }
    
}

void gestione_proiettile(int fd_alleati)
{
    int spostamento = -1;
    while (true)
    {
        write(fd_alleati,&spostamento , sizeof(spostamento)); //scrittura spostamento
        usleep(TIME_MAIN);
    }
}

void gestione_processi_proiettili_alleati(pid_t processi[N_MAX_P], int fd_alleati[N_MAX_P][2], oggetto_rana player)
{
    // Codice del processo padre
    pid_t child_pid;

    for (int i = 0; i < N_MAX_P; i++)
        {
            if(processi[i] > 0)
            {
                child_pid = waitpid(processi[i], NULL, WNOHANG) ;
                if (child_pid > 0) {
                    processi[i] = 0;
                }
            }
            else{
                 //il figlio è morto quindi chiama la funzione per ricrearlo
                CHECK_PID(processi[i]);

                if(processi[i] == 0){
                    close(fd_alleati[i][0]);
                    gestione_proiettile(fd_alleati[i][1]);
                }
                proiettili_alleati[i].x = player.x + 4;
                proiettili_alleati[i].y = player.y -1;
                proiettili_alleati[i].verso_proiettile = 1;
                close(fd_alleati[i][1]);

                i = N_MAX_P; 
            }
            

        }
}

void lettura_proiettili_alleati(int fd_alleati[N_MAX_P][2])
{
    int spostamento = 0;

    for (size_t i = 0; i < N_MAX_P; i++)
    {
        if(proiettili_alleati[i].x != -1)
        {
            read(fd_alleati[i][0],&spostamento, sizeof(spostamento));
            proiettili_alleati[i].y += spostamento;
        }
    }
}

void stampa_proiettili()
{
    
    for (size_t i = 0; i < N_MAX_P; i++)
    {
        init_pair(PROIETTILE,COLOR_RED, calcola_background(proiettili_alleati[i].x, proiettili_alleati[i].y)); //
        wattron(win_mappa, COLOR_PAIR(PROIETTILE));
        mvwprintw(win_mappa, proiettili_alleati[i].y, proiettili_alleati[i].x, "^");
        
    }
}