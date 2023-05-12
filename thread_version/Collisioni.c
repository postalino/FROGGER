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