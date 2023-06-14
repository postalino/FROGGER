#include "Strada.h"

void inizializza_veicoli()
{
    int primastrada=rand()%2;
    timer_cambio_corsia= ((rand()%100)+100);
  
    // inizializzo le strutture del veicolo per avere la stessa copia sia nel padre sia nel figlio
    for (int i = 0; i < N_VEICOLI; i++)
    {
        
        // do un timer per andare tutti sopra di una corsia ogni tot
        veicoli[i].timer = timer_cambio_corsia;
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

void genera_processi_veicoli(int fd_veicolo[N_VEICOLI][2],pid_t pid_veicoli[N_VEICOLI])
{
    for(int i =0;i<N_VEICOLI;i++)
    {
        CHECK_PIPE(fd_veicolo[i]);
        CHECK_PID(pid_veicoli[i]);
        if(!pid_veicoli[i])
        {   
            close(fd_veicolo[i][0]); // chiusura in lettura
            fcntl(fd_veicolo[i][0],F_SETFL, O_NONBLOCK);
            gestione_veicolo(fd_veicolo[i][1]);
        }
        close(fd_veicolo[i][1]);

    }
}

void gestione_veicolo(int fd)
{
    int spostamento = -1;
    while(true)
    {
        write(fd, &spostamento,sizeof(int));
        usleep(TIME_MAIN);
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
        mvwprintw(win_mappa,24+i,INIZIOSTRADA,"                              ");
        mvwprintw(win_mappa,24+i,FINESTRADA,"                               ");
    }
    
}

int check_v(int strada,int id, int n_X)
{
    for (size_t i = 0; i < N_VEICOLI; i++)
    {
        if(veicoli[i].y == strada)
        {
            if(veicoli[id].id_sprite == 1 || veicoli[id].id_sprite == 4) //veicolo da check è macchina
            {
                if(veicoli[i].id_sprite == 1 || veicoli[i].id_sprite == 4)  //veicolo è macchina
                {
                    for (size_t l = 0; l < 14; l++)
                    {
                        if(n_X + l >= veicoli[i].x && n_X + l <= veicoli[i].x + 14)
                        {
                            return 1;
                        }
                    }
                }else //veicolo è camion/bus
                {
                    for (size_t l = 0; l < 14; l++)
                    {
                        if(n_X + l >= veicoli[i].x && n_X + l <= veicoli[i].x + 28)
                        {
                            return 1;
                        }
                    }
                }
            }else //veicolo da check è camion/bus
            {
                if(veicoli[i].id_sprite == 1 || veicoli[i].id_sprite == 4)  //veicolo è macchina
                {
                    for (size_t l = 0; l < 28; l++)
                    {
                        if(n_X + l >= veicoli[i].x && n_X + l <= veicoli[i].x + 14)
                        {
                            return 1;
                        }
                    }
                }else //veicolo è camion/bus
                {
                    for (size_t l = 0; l < 28; l++)
                    {
                        if(n_X + l >= veicoli[i].x && n_X + l <= veicoli[i].x + 28)
                        {
                            return 1;
                        }
                    }
                }
            }
        }
    }
    return 0;
}

void aggiorna_veicoli(int fd[N_VEICOLI][2])
{
    int spostamento;
    for (size_t i = 0; i < N_VEICOLI; i++)
    {
        read(fd[i][0],&spostamento,sizeof(int));
        veicoli[i].timer--;
        veicoli[i].x += spostamento*veicoli[i].verso;

        if(veicoli[i].x >= FINESTRADA && veicoli[i].verso == -1)
        {
            if(veicoli[i].timer > 0 && check_v(veicoli[i].y,i,INIZIOSTRADA) == 0)
            {
                veicoli[i].id_sprite = rand()%3+1;
                veicoli[i].x = INIZIOSTRADA;
            }else
            {
                switch (veicoli[i].y)
                {
                case STRADA_1:
                    if(check_v(STRADA_2,i,FINESTRADA) == 0)
                    {
                        veicoli[i].timer = timer_cambio_corsia;
                        veicoli[i].x = FINESTRADA;
                        veicoli[i].y = STRADA_2;
                        veicoli[i].verso *= -1;
                        veicoli[i].id_sprite = rand()%3+4;
                    }
                    break;
                case STRADA_2:
                    if(check_v(STRADA_3,i,FINESTRADA) == 0)
                    {
                        veicoli[i].timer = timer_cambio_corsia;
                        veicoli[i].x = FINESTRADA;
                        veicoli[i].y = STRADA_3;
                        veicoli[i].verso *= -1;
                        veicoli[i].id_sprite = rand()%3+4;
                    }
                    
                    break;
                case STRADA_3:
                    if(check_v(STRADA_1,i,INIZIOSTRADA) == 0)
                    {
                        veicoli[i].timer = timer_cambio_corsia;
                        veicoli[i].x = INIZIOSTRADA;
                        veicoli[i].y = STRADA_1;
                        veicoli[i].id_sprite = rand()%3+1;
                    }
                    break;
                }
            }
        }
        else if(veicoli[i].x <= 0 && veicoli[i].verso == 1)
        {
            if(veicoli[i].timer > 0 && check_v(veicoli[i].y,i,FINESTRADA) == 0)
            {
                veicoli[i].id_sprite = rand()%3+4;
                veicoli[i].x = FINESTRADA;
            }else
            {
                switch (veicoli[i].y)
                {
                case STRADA_1:
                    if(check_v(STRADA_2,i,INIZIOSTRADA) == 0)
                    {
                        veicoli[i].timer = timer_cambio_corsia;
                        veicoli[i].x = INIZIOSTRADA;
                        veicoli[i].y = STRADA_2;
                        veicoli[i].verso *= -1;
                        veicoli[i].id_sprite = rand()%3+1;
                    }
                    break;
                case STRADA_2:
                    if(check_v(STRADA_3,i,INIZIOSTRADA) == 0)
                    {
                        veicoli[i].timer = timer_cambio_corsia;
                        veicoli[i].x = INIZIOSTRADA;
                        veicoli[i].y = STRADA_3;
                        veicoli[i].verso *= -1;
                        veicoli[i].id_sprite = rand()%3+1;
                    }
                    break;
                case STRADA_3:
                    if(check_v(STRADA_3,i,FINESTRADA) == 0)
                    {
                        veicoli[i].timer = timer_cambio_corsia;
                        veicoli[i].x = FINESTRADA;
                        veicoli[i].y = STRADA_1;
                        veicoli[i].id_sprite = rand()%3+4;
                    }
                    break;
                }
            }
        }
        /*
        if(veicoli[i].x == 129 && veicoli[i].verso == -1)// veicolo arrivato a fine strada destra
        {
            veicoli[i].id_sprite = rand()%3+1;
            if (veicoli[i].timer <= 0)
            {
                veicoli[i].timer = timer_cambio_corsia;
                switch (veicoli[i].y)
                {
                case STRADA_1:
                    veicoli[i].y = STRADA_2;
                    veicoli[i].verso *= -1;
                    veicoli[i].id_sprite = rand()%3+4;
                    veicoli[i].x = 129;
                    break;
                case STRADA_2:
                    veicoli[i].y = STRADA_3;
                    veicoli[i].verso *= -1;
                    veicoli[i].id_sprite = rand()%3+4;
                    veicoli[i].x = 129;
                    break;
                case STRADA_3:
                    veicoli[i].y = STRADA_1;
                    veicoli[i].x = 0;
                    break;
                }
            }else
            {
                veicoli[i].x = 0;
            }
        }else if(veicoli[i].x == 0 && veicoli[i].verso == 1)
        {
            veicoli[i].id_sprite = rand()%3+4;
            if (veicoli[i].timer <= 0)
            {
                
                veicoli[i].timer = timer_cambio_corsia;
                    
                switch (veicoli[i].y)
                {
                case STRADA_1:
                    veicoli[i].y = STRADA_2;
                    veicoli[i].id_sprite = rand()%3+1;
                    veicoli[i].x = 0;
                    veicoli[i].verso *= -1;
                    break;
                case STRADA_2:
                    veicoli[i].y = STRADA_3;
                    veicoli[i].id_sprite = rand()%3+1;
                    veicoli[i].x = 0;
                    veicoli[i].verso *= -1;
                    break;
                case STRADA_3:
                    veicoli[i].y = STRADA_1;
                    veicoli[i].x = 129;
                    break;
                }
            }else
            {
                veicoli[i].x = 129;
            }
        } */ 
    }
}

void cambio_direzione()
{
    for (size_t i = 0; i < N_VEICOLI; i++)
    {
        veicoli[i].verso *= -1;
        switch (veicoli[i].id_sprite)
        {
        case 1:
            veicoli[i].id_sprite=4;
            break;
        case 2:
            veicoli[i].id_sprite=5;
            break;
        case 3:
            veicoli[i].id_sprite=6;
            break;
        case 4:
            veicoli[i].id_sprite=1;
            break;
        case 5:
            veicoli[i].id_sprite=2;
            break;
        case 6:
            veicoli[i].id_sprite=3;
            break;
        }
    }
    
}

    