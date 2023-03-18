#include "Collisioni.h"

void tempoDiGioco(int fd)
{
    int clock = 0;
    int tempo = 0;
    while (tempo <= 60)
    {
        sleep(1);
        clock++;
        write(fd,&clock , sizeof(clock)); //scritture tempo di gioco rimanente
        tempo++;
        clock--;
    }
}