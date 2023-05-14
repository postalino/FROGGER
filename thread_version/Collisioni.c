#include "Collisioni.h"

void* tempoDiGioco()
{
    while (running)
    {
        sleep(1);
        max_time--;
    }
}

void inizializza_proiettili()
{
    for (size_t i = 0; i < N_MAX_P; i++)
    {
        proiettili_alleati[i].x = -1;
        proiettili_nemici[i].x = -1;
    }   
}

void game_over()
{
    if(max_time < 0 || vite <1)
    {
        pthread_mutex_lock (&semCurses);
        running = false;
        pthread_mutex_unlock (&semCurses);
    }  
}

void collisioni_rana_veicoli()
{
    for (int i=0;i< N_VEICOLI;i++)
    {
        if (veicolo[i].id_sprite == 1 || veicolo[i].id_sprite == 4) //hitbox macchina
        {
            for(int j=0;j<14;j++)
            {
                if ((player.x == veicolo[i].x + j && player.y == veicolo[i].y) || (player.x + 8 == veicolo[i].x + j && player.y == veicolo[i].y))
                {
                    //pthread_mutex_lock (&semCurses);
                    vite--;
                    finemanche = 6;
                    player.x = X_START;
                    player.y = Y_START;
                    //pthread_mutex_unlock (&semCurses);
                }
            }
        }else //hitbox camion/pullman
        {
            for(int j=0;j<28;j++)
            {
                if ((player.x == veicolo[i].x + j && player.y == veicolo[i].y) || (player.x + 8 == veicolo[i].x + j && player.y == veicolo[i].y))
                {
                    //pthread_mutex_lock (&semCurses);
                    vite--;
                    finemanche = 6;
                    player.x = X_START;
                    player.y = Y_START;
                    //pthread_mutex_unlock (&semCurses);
                }
            }
        }
    }
}

void* gestione_proiettili_A(void* Proiettile)
{
    oggetto_proiettile* P = (oggetto_proiettile*) Proiettile;
    while(running)
    {
        if (P->x != -1)
        {
            pthread_mutex_lock (&semCurses);
            P->y--;
            if (P->y < 0)
            {
                P->x = -1;
            }
            pthread_mutex_unlock (&semCurses);
        }
        usleep(TIME_MAIN);
    }
}

void stampa_proiettili()
{
    for (size_t i = 0; i < N_MAX_P; i++)
    {
        if(proiettili_alleati[i].x != -1)
        {
            init_pair(PROIETTILE + i,COLOR_RED, calcola_background(proiettili_alleati[i].x, proiettili_alleati[i].y));
            wattron(win_mappa, COLOR_PAIR(PROIETTILE + i));
            mvwprintw(win_mappa, proiettili_alleati[i].y, proiettili_alleati[i].x, "^");
            wattroff(win_mappa,COLOR_PAIR(PROIETTILE + i));
        }
        if(proiettili_nemici[i].x != -1){
            init_pair(PROIETTILE + N_MAX_P + i,COLOR_RED, calcola_background(proiettili_nemici[i].x, proiettili_nemici[i].y)); 
            wattron(win_mappa, COLOR_PAIR(PROIETTILE + N_MAX_P + i));
            mvwprintw(win_mappa, proiettili_nemici[i].y, proiettili_nemici[i].x, "v");
            wattroff(win_mappa,COLOR_PAIR(PROIETTILE + N_MAX_P + i));
        }
    }
}
