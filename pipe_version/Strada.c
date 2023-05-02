#include "Strada.h"

void inizializza_veicoli()
{
    srand(getpid());
    int primastrada=rand()%2;
    int cambio_corsia= ((rand()%1000)+1000);
  
    // inizializzo le strutture del veicolo per avere la stessa copia sia nel padre sia nel figlio
    for (int i = 0; i < N_VEICOLI; i++)
    {
        
        // do un timer per andare tutti sopra di una corsia ogni tot
        veicoli[i].timer = cambio_corsia;
        // do a u tutti i veicoli un verso dipende da prima strada per essere diverso ad ogni esecuzione
        if (primastrada == 0)
        {
           if (i == 2 || i == 3)
            {
                //corsia centrale sinistra
                veicoli[i].verso = 1;
                veicoli[i].id_sprite = rand()%3+4;
            }else
            {   
                //corsie laterali destra
                veicoli[i].verso = -1;
                veicoli[i].id_sprite = rand()%3+1;
            }
        }else
        {
            if (i == 2 || i == 3)
            {
                //corsia centrale destra
                veicoli[i].verso = -1;
                veicoli[i].id_sprite = rand()%3+1;
            }else
            {   
                //corsie laterali sinistra
                veicoli[i].verso = 1;
                veicoli[i].id_sprite = rand()%3+4;
            }
        }
        // do una x iniziale a tutti i veicoli
        if(i%2==0)
        {
            if(veicoli[i].verso == -1)
            {
                veicoli[i].x = 20;
            }else
            {
                veicoli[i].x = 125;
            }
        }else
        {
            if(veicoli[i].verso == -1)
            {
                veicoli[i].x = 82;
            }else
            {
                veicoli[i].x = 76;
            }
        }
        // do una y iniziale a tutti i veicoli
        if (i < 2)
        {
            veicoli[i].y= STRADA_1;
        }else if (i < 4)
        {
            veicoli[i].y = STRADA_2;
        }else
        {
            veicoli[i].y = STRADA_3;
        }
    }
}

void genera_processi_veicoli(int fd_veicolo[N_VEICOLI][2],pid_t pid_veicoli[N_VEICOLI], int fd_fine_manche[N_VEICOLI][2])
{
    for(int i =0;i<N_VEICOLI;i++)
    {
        CHECK_PIPE(fd_veicolo[i]);
        CHECK_PIPE(fd_fine_manche[i]);
        CHECK_PID(pid_veicoli[i]);
        if(!pid_veicoli[i])
        {   
            close(fd_veicolo[i][0]); // chiusura in lettura
            close(fd_fine_manche[i][1]);
            fcntl(fd_fine_manche[i][0],F_SETFL, O_NONBLOCK);
            gestione_veicolo(fd_veicolo[i][1],i,fd_fine_manche[i][0]);
        }
        close(fd_veicolo[i][1]);
        close(fd_fine_manche[i][0]);

    }
}

void gestione_veicolo(int fd,int id,int fd_fine_manche)
{
    srand(getpid());
    int time = veicoli[id].timer;
    int finemanche=0;
    while (true)
    {
        usleep(TIME_MAIN);
        read(fd_fine_manche,&finemanche,sizeof(finemanche));
        if (finemanche == 1)
        {
            veicoli[id].verso *= -1;
            switch (veicoli[id].id_sprite)
            {
            case 1:
                veicoli[id].id_sprite=4;
                break;
            case 2:
                veicoli[id].id_sprite=5;
                break;
            case 3:
                veicoli[id].id_sprite=6;
                break;
            case 4:
                veicoli[id].id_sprite=1;
                break;
            case 5:
                veicoli[id].id_sprite=2;
                break;
            case 6:
                veicoli[id].id_sprite=3;
                break;
            }
            finemanche =0;
        }
        time--;

        if (veicoli[id].verso == -1)
        {
            veicoli[id].x++;
            write(fd, &veicoli[id],sizeof(veicoli[id]));
        }else
        {
            veicoli[id].x--;
            write(fd, &veicoli[id],sizeof(veicoli[id]));
        }

        if(veicoli[id].x == 129 && veicoli[id].verso == -1)
        {
            veicoli[id].id_sprite = rand()%3+1;
            if (time <= 0)
            {
                veicoli[id].x = 129;
                veicoli[id].verso *= -1;
                time = veicoli[id].timer;
                veicoli[id].id_sprite = rand()%3+4;
                switch (veicoli[id].y)
                {
                case STRADA_1:
                    veicoli[id].y = STRADA_2;
                    break;
                case STRADA_2:
                    veicoli[id].y = STRADA_3;
                    break;
                case STRADA_3:
                    veicoli[id].y = STRADA_1;
                    break;
                }
            }else
            {
                veicoli[id].x = 0;
            }
        }else if(veicoli[id].x == 0 && veicoli[id].verso == 1)
        {
            veicoli[id].id_sprite = rand()%3+4;
            if (time <= 0)
            {
                veicoli[id].x = 29;
                veicoli[id].verso *= -1;
                time = veicoli[id].timer;
                veicoli[id].id_sprite = rand()%3+1;
                switch (veicoli[id].y)
                {
                case STRADA_1:
                    veicoli[id].y = STRADA_2;
                    break;
                case STRADA_2:
                    veicoli[id].y = STRADA_3;
                    break;
                case STRADA_3:
                    veicoli[id].y = STRADA_1;
                    break;
                }
            }else
            {
                veicoli[id].x = 129;
            }
        }
    } 
}

void stampa_veicoli()
{
    init_pair(VEICOLO,COLOR_WHITE, COLOR_BLACK);
    for (size_t i = 0; i < N_VEICOLI; i++)
    {
        switch (veicoli[i].id_sprite)
        {
        case 1:
            wattron(win_mappa,COLOR_PAIR(VEICOLO));
            print_sprite(veicoli[i].x,veicoli[i].y,CAR_RIGHT);
            break;
                    case 2:
            wattron(win_mappa,COLOR_PAIR(VEICOLO));
            print_sprite(veicoli[i].x,veicoli[i].y,BUS_RIGHT);
            break;
                    case 3:
            wattron(win_mappa,COLOR_PAIR(VEICOLO));
            print_sprite(veicoli[i].x,veicoli[i].y,TRUCK_RIGHT);
            break;
                    case 4:
            wattron(win_mappa,COLOR_PAIR(VEICOLO));
            print_sprite(veicoli[i].x,veicoli[i].y,CAR_LEFT);
            break;
                    case 5:
            wattron(win_mappa,COLOR_PAIR(VEICOLO));
            print_sprite(veicoli[i].x,veicoli[i].y,BUS_LEFT);
            break;
                    case 6:
            wattron(win_mappa,COLOR_PAIR(VEICOLO));
            print_sprite(veicoli[i].x,veicoli[i].y,TRUCK_LEFT);
            break;
        }
    }
    for (size_t i = 0; i < 12; i++)
    {
        wattron(win_mappa,COLOR_PAIR(VEICOLO));
        mvwprintw(win_mappa,24+i,0,"                              ");
        mvwprintw(win_mappa,24+i,129,"                               ");
    }
    
}
void aggiorna_veicoli(int fd[N_VEICOLI][2])
{
    oggetto_veicolo new;
    for (size_t i = 0; i < N_VEICOLI; i++)
    {
        read(fd[i][0],&new,sizeof(oggetto_veicolo));
        veicoli[i].id_sprite = new.id_sprite;
        veicoli[i].verso = new.verso;
        veicoli[i].x = new.x;
        veicoli[i].y = new.y;
    }
}