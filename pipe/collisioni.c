
#include "gestione_grafica.h"
#include "strada.h"
#include "collisioni.h"

int collisioni_rana_veicoli(oggetto_rana rana,oggetto_veicolo veicolo [N_VEICOLI])
{
    for (int i=0;i< N_VEICOLI;i++)
    {
        if (veicolo[i].id_sprite == 1 || veicolo[i].id_sprite == 4) //hitbox macchina
        {
            for(int j=0;j<14;j++)
            {
                if ((rana.x == veicolo[i].x + j && rana.y == veicolo[i].y) || (rana.x + 8 == veicolo[i].x + j && rana.y == veicolo[i].y))
                {
                    return 1;
                }
            }
        }else //hitbox camion/pullman
        {
            for(int j=0;j<28;j++)
            {
                if ((rana.x == veicolo[i].x + j && rana.y == veicolo[i].y) || (rana.x + 8 == veicolo[i].x + j && rana.y == veicolo[i].y))
                {
                    return 1;
                }
            }
        }
    }

    return 0;
}

int genera_processo_proiettile(int fd[2],oggetto_rana npc_rana)
{
    int Rpid;
    pid_t Pid_proiettile;
    CHECK_PID(Pid_proiettile);
    CHECK_PIPE(fd);
    if (!Pid_proiettile)
    {   
        close(fd[0]);
        muovi_proiettile(fd[1],npc_rana);
        Rpid = getpid();
    }
    mvprintw(0,MAXX/2,"%d %d",npc_rana.x,npc_rana.y);
    return Rpid;
}

void muovi_proiettile(int fd,oggetto_rana npc_rana)
{
    oggetto_proiettile proiettile={0,0,0,0,0};
    proiettile.x = npc_rana.x + 4;
    proiettile.y = npc_rana.y + 1;

    while (true)
    {
        write(fd,&proiettile,sizeof(oggetto_proiettile));
        proiettile.y ++;
    }
}

void stampa_proiettili(oggetto_proiettile proiettili [MAX_PROIETTILI])
{
    for (int i =0; i<MAX_PROIETTILI;i++)
    {
        if (proiettili[i].sparato == 1)
        {
            mvprintw(proiettili[i].y,proiettili[i].x,"^");
        }
        
    }
}

void aggiorna_proiettile(int fd,oggetto_proiettile* proiettile)
{
    oggetto_proiettile input;
    read(fd,&input,sizeof(oggetto_proiettile));
    proiettile->x = input.x;
    proiettile->y = input.y;
}