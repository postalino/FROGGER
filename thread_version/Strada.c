#include "Strada.h"

void inizializza_veicoli()
{
    int primastrada=rand()%2;
    int cambio_corsia= ((rand()%1000)+1000);
    for (size_t i = 0; i < N_VEICOLI; i++)
        {
            finemanche[i]=0;
        }
    
  
    // inizializzo le strutture del veicolo per avere la stessa copia sia nel padre sia nel figlio
    for (int i = 0; i < N_VEICOLI; i++)
    {
        veicolo[i].id = i;
        // do un timer per andare tutti sopra di una corsia ogni tot
        veicolo[i].timer = cambio_corsia;
        // do a u tutti i veicoli un verso dipende da prima strada per essere diverso ad ogni esecuzione
        if (primastrada == 0)
        {
           if (i == 2 || i == 3)
            {
                //corsia centrale sinistra
                veicolo[i].verso = 1;
                veicolo[i].id_sprite = rand()%3+4;
            }else
            {   
                //corsie laterali destra
                veicolo[i].verso = -1;
                veicolo[i].id_sprite = rand()%3+1;
            }
        }else
        {
            if (i == 2 || i == 3)
            {
                //corsia centrale destra
                veicolo[i].verso = -1;
                veicolo[i].id_sprite = rand()%3+1;
            }else
            {   
                //corsie laterali sinistra
                veicolo[i].verso = 1;
                veicolo[i].id_sprite = rand()%3+4;
            }
        }
        // do una x iniziale a tutti i veicoli
        if(i%2==0)
        {
            if(veicolo[i].verso == -1)
            {
                veicolo[i].x = 20;
            }else
            {
                veicolo[i].x = 125;
            }
        }else
        {
            if(veicolo[i].verso == -1)
            {
                veicolo[i].x = 82;
            }else
            {
                veicolo[i].x = 76;
            }
        }
        // do una y iniziale a tutti i veicoli
        if (i < 2)
        {
            veicolo[i].y= STRADA_1;
        }else if (i < 4)
        {
            veicolo[i].y = STRADA_2;
        }else
        {
            veicolo[i].y = STRADA_3;
        }
    }
}

void* gestione_veicolo(void* veicolo)
{
    oggetto_veicolo* veicoli = (oggetto_veicolo *) veicolo;
    srand(getpid());
    int time = veicoli->timer;
    while (running)
    {
        usleep(TIME_MAIN);
        if (finemanche[veicoli->id]==1)
        {
            pthread_mutex_lock (&semCurses);
            veicoli->verso *= -1;
            switch (veicoli->id_sprite)
            {
            case 1:
                veicoli->id_sprite=4;
                break;
            case 2:
                veicoli->id_sprite=5;
                break;
            case 3:
                veicoli->id_sprite=6;
                break;
            case 4:
                veicoli->id_sprite=1;
                break;
            case 5:
                veicoli->id_sprite=2;
                break;
            case 6:
                veicoli->id_sprite=3;
                break;
            }
            finemanche[veicoli->id]=0;
            pthread_mutex_unlock (&semCurses);
        }
        time--;

        if (veicoli->verso == -1)
        {
            pthread_mutex_lock (&semCurses);
            veicoli->x++;
            pthread_mutex_unlock (&semCurses);
        }else
        {
            pthread_mutex_lock (&semCurses);
            veicoli->x--;
            pthread_mutex_unlock (&semCurses);
        }

        if(veicoli->x == 129 && veicoli->verso == -1)
        {
            pthread_mutex_lock (&semCurses);
            veicoli->id_sprite = rand()%3+1;
            if (time <= 0)
            {
                
                time = veicoli->timer;
                
                switch (veicoli->y)
                {
                case STRADA_1:
                    veicoli->y = STRADA_2;
                    veicoli->x = 129;
                    veicoli->verso *= -1;
                    veicoli->id_sprite = rand()%3+4;
                    break;
                case STRADA_2:
                    veicoli->y = STRADA_3;
                    veicoli->x = 129;
                    veicoli->verso *= -1;
                    veicoli->id_sprite = rand()%3+4;
                    break;
                case STRADA_3:
                    veicoli->y = STRADA_1;
                    veicoli->x = 0;
                    break;
                }
            }else
            {
                veicoli->x = 0;
            }
            pthread_mutex_unlock (&semCurses);
        }else if(veicoli->x == 0 && veicoli->verso == 1)
        {
            pthread_mutex_lock (&semCurses);
            veicoli->id_sprite = rand()%3+4;
            if (time <= 0)
            {
                time = veicoli->timer;
                switch (veicoli->y)
                {
                case STRADA_1:
                    veicoli->y = STRADA_2;
                    veicoli->x = 0;
                    veicoli->verso *= -1;
                    veicoli->id_sprite = rand()%3+1;
                    break;
                case STRADA_2:
                    veicoli->y = STRADA_3;
                    veicoli->x = 0;
                    veicoli->verso *= -1;
                    veicoli->id_sprite = rand()%3+1;
                    break;
                case STRADA_3:
                    veicoli->y = STRADA_1;
                    veicoli->x = 129;
                    break;
                }
            }else
            {
                veicoli->x = 129;
            }
            pthread_mutex_unlock (&semCurses);
        }
    } 
}

void stampa_veicoli()
{
    init_pair(VEICOLO,COLOR_WHITE, COLOR_BLACK);
    for (size_t i = 0; i < N_VEICOLI; i++)
    {
        switch (veicolo[i].id_sprite)
        {
        case 1:
            wattron(win_mappa,COLOR_PAIR(VEICOLO));
            print_sprite(veicolo[i].x,veicolo[i].y,CAR_RIGHT);
            break;
                    case 2:
            wattron(win_mappa,COLOR_PAIR(VEICOLO));
            print_sprite(veicolo[i].x,veicolo[i].y,BUS_RIGHT);
            break;
                    case 3:
            wattron(win_mappa,COLOR_PAIR(VEICOLO));
            print_sprite(veicolo[i].x,veicolo[i].y,TRUCK_RIGHT);
            break;
                    case 4:
            wattron(win_mappa,COLOR_PAIR(VEICOLO));
            print_sprite(veicolo[i].x,veicolo[i].y,CAR_LEFT);
            break;
                    case 5:
            wattron(win_mappa,COLOR_PAIR(VEICOLO));
            print_sprite(veicolo[i].x,veicolo[i].y,BUS_LEFT);
            break;
                    case 6:
            wattron(win_mappa,COLOR_PAIR(VEICOLO));
            print_sprite(veicolo[i].x,veicolo[i].y,TRUCK_LEFT);
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