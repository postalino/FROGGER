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
