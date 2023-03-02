#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdbool.h>
#include "gestione_grafica.h"
#include "strada.h"

void genera_coord_veicolo_right(int fd,int x,int y)
{
    srand(getpid());
    int sprite = (rand()%3)+1;
    oggetto_veicolo veicol = {x,y,sprite};
    write(fd,&veicol,sizeof(oggetto_veicolo));
    while(true)
    {
        veicol.x ++;
        write(fd,&veicol,sizeof(oggetto_veicolo));
        if (veicol.x == MAXX)
        {
            veicol.x = 0;
            veicol.id_sprite = (rand()%3)+1;
        }
        
    }

}

void genera_coord_veicolo_left(int fd,int x,int y)
{
    srand(getpid());
    int sprite = (rand()%3)+4;
    oggetto_veicolo veicol = {x,y,sprite};
    write(fd,&veicol,sizeof(oggetto_veicolo));
    while(true)
    {
        veicol.x --;
        write(fd,&veicol,sizeof(oggetto_veicolo));
        if (veicol.x == 0)
        {
            veicol.x = MAXX;
            veicol.id_sprite = (rand()%3)+4;
        }
       
    }
    

}

void genera_processi_veicoli(int fd_veicolo[N_VEICOLI][2],pid_t pid_veicoli[N_VEICOLI])
{

    srand(getpid());
    int prima_strada=rand()%2; //variabile per decidere se la prima strada andrà verso destra o verso sinistra

    for(int i =0;i<N_VEICOLI;i++)
    {
        CHECK_PIPE(fd_veicolo[i]);
        CHECK_PID(pid_veicoli[i]);
        if(!pid_veicoli[i])
        {
            close(fd_veicolo[i][0]);
            if (prima_strada == 0)
            {
                switch (i)
                {
                case 0:
                    genera_coord_veicolo_right(fd_veicolo[i][1],0,STRADA_1);
                    break;
                case 1:
                    genera_coord_veicolo_right(fd_veicolo[i][1],MAXX/2,STRADA_1);
                    break;
                case 2:
                    genera_coord_veicolo_left(fd_veicolo[i][1],MAXX,STRADA_2);
                    break;
                case 3:
                    genera_coord_veicolo_left(fd_veicolo[i][1],MAXX/2,STRADA_2);
                    break;
                case 4:
                    genera_coord_veicolo_right(fd_veicolo[i][1],0,STRADA_3);
                    break;
                case 5:
                    genera_coord_veicolo_right(fd_veicolo[i][1],MAXX/2,STRADA_3);
                    break;
                }
            }else
            {
                switch (i)
                {
                case 0:
                    genera_coord_veicolo_left(fd_veicolo[i][1],MAXX,STRADA_1);
                    break;
                case 1:
                    genera_coord_veicolo_left(fd_veicolo[i][1],MAXX/2,STRADA_1);
                    break;
                case 2:
                    genera_coord_veicolo_right(fd_veicolo[i][1],0,STRADA_2);
                    break;
                case 3:
                    genera_coord_veicolo_right(fd_veicolo[i][1],MAXX/2,STRADA_2);
                    break;
                case 4:
                    genera_coord_veicolo_left(fd_veicolo[i][1],MAXX,STRADA_3);
                    break;
                case 5:
                    genera_coord_veicolo_left(fd_veicolo[i][1],MAXX/2,STRADA_3);
                    break;
                }
            }
        }
    }
}
void stampa_veicoli(oggetto_veicolo veicolo[])
{
    init_pair(11,COLOR_WHITE,COLOR_BLACK);
    attron(COLOR_PAIR(11));
    for(int i = 0; i < N_VEICOLI;i++)
    {
        switch (veicolo[i].id_sprite)
        {
        case 1:
            print_sprite(veicolo[i].x,veicolo[i].y,CAR_RIGHT,DIM_CAR);
            break;
        case 4:
            print_sprite(veicolo[i].x,veicolo[i].y,CAR_LEFT,DIM_CAR);
            break;
        case 2:
            print_sprite(veicolo[i].x,veicolo[i].y,BUS_RIGHT,DIM_BUS);
            break;
        case 5:
            print_sprite(veicolo[i].x,veicolo[i].y,BUS_LEFT,DIM_BUS);
            break;
        case 3:
            print_sprite(veicolo[i].x,veicolo[i].y,TRUCK_RIGHT,DIM_TRUCK);
            break;
        case 6:
            print_sprite(veicolo[i].x,veicolo[i].y,TRUCK_LEFT,DIM_TRUCK);
            break;
        }
    }
   

}