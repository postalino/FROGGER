#include "Frogger.h"
#define TIME 170000

void gestione_tronco(int fd_tronco, int corsia)
{ 
    srand(getpid());
    int verso;
    int dimensione_tronco;

    //determino randomicamente il verso del tronco ( 0 -. destra | 1 -. sinistra)
    verso = rand()%2;

    //determino randomicamente la dimensione del tronco (0 -. x2 | 1 -. x3)
    dimensione_tronco = rand()%2;
    
    //caso partanza da destra con dimensione x2
    if(!verso && !dimensione_tronco)
    {
        //posiziona il tronco nelle rispettive cordinate iniziali della rispettiva corsia
        oggetto_tronco npc_tronco = {MAXX - L_TRUNK_X2,H_FROGGER * (2 + corsia), ID_TRUNK_X2, corsia};
        int direzione = -L_FROGGER;
        npc_tronco.verso = -1;

        while (true)
        {
            write(fd_tronco, &npc_tronco, sizeof(oggetto_tronco));//scrittura posizione nella processo_tronco
            npc_tronco.x += direzione;

            if(npc_tronco.x + L_TRUNK_X2 >= MAXX) //cambia la direzione del tronco se tocca il bordo destro
            {
                direzione *= -1;
            }
            else if(npc_tronco.x <= 0)//cambia la direzione del tronco se tocca il bordo sinistro
            {
                direzione *= -1;
            }

            if(direzione > 0)
                npc_tronco.verso = 1;
            else if(direzione < 0)
                npc_tronco.verso = -1;

            usleep(TIME);
        } 
    }

    //caso patenza sinistra con dimensione x2
    else if(verso && !dimensione_tronco)
    {
        //posiziona il tronco nelle rispettive cordinate iniziali della rispettiva corsia
        oggetto_tronco npc_tronco = {0, H_FROGGER * (2 + corsia), ID_TRUNK_X2, corsia};
        int direzione = +L_FROGGER;
        npc_tronco.verso= 1;

        while (true)
        {
            write(fd_tronco, &npc_tronco, sizeof(oggetto_tronco));//scrittura posizione nella processo_tronco
            npc_tronco.x += direzione;
            if(npc_tronco.x + L_TRUNK_X2 >= MAXX) //cambia la direzione del tronco se tocca il bordo destro
            {
                direzione *= -1;
            }
            else if(npc_tronco.x <= 0)//cambia la direzione del tronco se tocca il bordo sinistro
            {
                direzione *= -1;
            }

            if(direzione > 0)
                npc_tronco.verso = 1;
            else if(direzione < 0)
                npc_tronco.verso = -1;
            
            usleep(TIME);
        }
    }
    
    //caso partanza da destra con dimensione x3
    else if(!verso && dimensione_tronco)
    {
        //posiziona il tronco nelle rispettive cordinate iniziali della rispettiva corsia
        oggetto_tronco npc_tronco = {MAXX - L_TRUNK_X3, H_FROGGER * (2 + corsia), ID_TRUNK_X3, corsia};
        int direzione = -L_FROGGER;
        npc_tronco.verso= -1;

        while(true)
        {
            write(fd_tronco, &npc_tronco, sizeof(oggetto_tronco));//scrittura posizione nella processo_tronco
            npc_tronco.x += direzione;
            if(npc_tronco.x + L_TRUNK_X3 >= MAXX) //cambia la direzione del tronco se tocca il bordo destro
            {
                direzione *= -1;
            }
            else if(npc_tronco.x <= 0)//cambia la direzione del tronco se tocca il bordo sinistro
            {
                direzione *= -1;
            }

            if(direzione > 0)
                npc_tronco.verso = 1;
            else if(direzione < 0)
                npc_tronco.verso = -1;

            usleep(TIME);
        }
    }

    //caso patenza sinistra con dimensione x3
    else if(verso && dimensione_tronco)
    {
        //posiziona il tronco nelle rispettive cordinate iniziali della rispettiva corsia
        oggetto_tronco npc_tronco = {0, H_FROGGER * (2 + corsia), ID_TRUNK_X3, corsia};
        int direzione = +L_FROGGER;
        npc_tronco.verso = +1;

        while (true)
        {
            write(fd_tronco, &npc_tronco, sizeof(oggetto_tronco));//scrittura posizione nella processo_tronco
            npc_tronco.x += direzione;
            
            if(npc_tronco.x + L_TRUNK_X3 >= MAXX) //cambia la direzione del tronco se tocca il bordo destro
            {
                direzione *= -1;
            }
            else if(npc_tronco.x <= 0)//cambia la direzione del tronco se tocca il bordo sinistro
            {
                direzione *= -1;
            }
            
            if(direzione > 0)
                npc_tronco.verso = 1;
            else if(direzione < 0)
                npc_tronco.verso = -1;

            usleep(TIME);
        }
    }
}


void lettura_pipe_tronchi(oggetto_tronco tronchi[N_CORSIE_FIUME], int fd_tronco[N_CORSIE_FIUME][2])
{
    for (size_t i = 0; i < N_CORSIE_FIUME; i++)
        {
            read(fd_tronco[i][0],&tronchi[i],sizeof(oggetto_tronco));
        }
}

void stampa_tronchi (oggetto_tronco tronchi[N_CORSIE_FIUME])
{
    for (size_t i = 0; i < N_CORSIE_FIUME; i++)
        {
            if(tronchi[i].id_sprite == ID_TRUNK_X2)
            {
                attron(COLOR_PAIR(5));
                print_sprite(tronchi[i].x, tronchi[i].y,TRUNK_X2, DIM_TRUNK_X2);
            }
            else if(tronchi[i].id_sprite == ID_TRUNK_X3)
            {
                attron(COLOR_PAIR(5));
                print_sprite(tronchi[i].x, tronchi[i].y,TRUNK_X3, DIM_TRUNK_X3);
            }
        }
}

void tronco_taxi(oggetto_rana *npc_rana, oggetto_tronco npc_tronco)
{ 
    if((npc_rana->x == npc_tronco.x) && (npc_rana->y == npc_tronco.y))
        {
            npc_rana->sentinella_trasporto = true;
            if(L_FROGGER * npc_tronco.verso > 0)
                npc_rana->verso = 1;//destra
            else if(L_FROGGER * npc_tronco.verso < 0)
                npc_rana->verso = -1;//sinistra
        }
    else if((npc_rana->x == npc_tronco.x +9) && (npc_rana->y == npc_tronco.y))
        {
            npc_rana->sentinella_trasporto = true;
            if(L_FROGGER * npc_tronco.verso > 0)
                npc_rana->verso = 1;//destra
            else if(L_FROGGER * npc_tronco.verso < 0)
                npc_rana->verso = -1;//sinistra
        }
    else if((npc_tronco.id_sprite == ID_TRUNK_X3) && (npc_rana->x == npc_tronco.x +18) && (npc_rana->y == npc_tronco.y))
        {
            npc_rana->sentinella_trasporto = true;
            if(L_FROGGER * npc_tronco.verso > 0)
                npc_rana->verso = 1;//destra
            else if(L_FROGGER * npc_tronco.verso < 0)
                npc_rana->verso = -1;//sinistra
        }
    else
    {
        npc_rana->sentinella_trasporto = false;
        npc_rana->verso = 0;
    }
    usleep(170000);

}

void funzione_tempo_gioco(WINDOW *wtime)
{
    int secondi=0;

    while (true)
    {
        mvwprintw(wtime, MAXY+2, 1, "TIME: %d", secondi);
        wrefresh(wtime);
        //sleep(1);
        secondi++;
    }
    
}