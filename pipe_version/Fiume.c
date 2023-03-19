#include "Fiume.h"
#define TIME 170000 //temporizzazione scrittura per gestire la velocità del tronco

void inizializza_tronco(oggetto_tronco *npc, int corsia, int verso, int dimensione_tronco)
{    
    //caso partanza da destra con dimensione x2
    if(!verso && !dimensione_tronco)
    {
        //posiziona il tronco nelle rispettive cordinate iniziali della rispettiva corsia
        npc->x = (MAXX-30) - L_TRUNK_X2;
        npc->y =H_FROGGER * (2 + corsia);
        npc->id_sprite = ID_TRUNK_X2;
        npc->corsia = corsia;
        npc->verso = -1;
    }

    //caso patenza sinistra con dimensione x2
    else if(verso && !dimensione_tronco)
    {
        //posiziona il tronco nelle rispettive cordinate iniziali della rispettiva corsia
        npc->x = 30;
        npc->y =H_FROGGER * (2 + corsia);
        npc->id_sprite = ID_TRUNK_X2;
        npc->corsia = corsia;
        npc->verso= +1;
    }
    
    //caso partanza da destra con dimensione x3
    else if(!verso && dimensione_tronco)
    {
        //posiziona il tronco nelle rispettive cordinate iniziali della rispettiva corsia
        npc->x = (MAXX-30) - L_TRUNK_X3;
        npc->y =H_FROGGER * (2 + corsia);
        npc->id_sprite = ID_TRUNK_X3;
        npc->corsia = corsia;
        npc->verso= -1;
    }

    //caso patenza sinistra con dimensione x3
    else if(verso && dimensione_tronco)
    {
        //posiziona il tronco nelle rispettive cordinate iniziali della rispettiva corsia
        npc->x = 30;
        npc->y =H_FROGGER * (2 + corsia);
        npc->id_sprite = ID_TRUNK_X3;
        npc->corsia = corsia;
        npc->verso = +1;
    }
}

void stampa_tronchi (oggetto_tronco tronchi[N_CORSIE_FIUME])
{
    init_pair(TRONCO,COLOR_WHITE,COLOR_BLUE); //palette tronco

    for (size_t i = 0; i < N_CORSIE_FIUME; i++)
        {
            if(tronchi[i].id_sprite == ID_TRUNK_X2)     //tronco da due
            {
                wattron(win_mappa,COLOR_PAIR(TRONCO));
                print_sprite(tronchi[i].x, tronchi[i].y,TRUNK_X2);
            }
            else if(tronchi[i].id_sprite == ID_TRUNK_X3)    //tronco da tre
            {
                wattron(win_mappa,COLOR_PAIR(TRONCO));
                print_sprite(tronchi[i].x, tronchi[i].y,TRUNK_X3);
            }
        }
}

void generazione_processi_tronco(int fd_tronchi[N_CORSIE_FIUME][2], pid_t processo_tronco[N_CORSIE_FIUME], oggetto_tronco npc[N_CORSIE_FIUME])
{
    for (int i = 0; i < N_CORSIE_FIUME; i++)
    {
        CHECK_PIPE(fd_tronchi[i]);//verifica se la pipe e' stata creata correttamente
        CHECK_PID(processo_tronco[i]);//creo processo figlio tronco
        if(!processo_tronco[i])
        {  
            //blocca la lettura nella pipe per il processo figlio
            close(fd_tronchi[i][0]);

            //funzione movimento tronco
            gestione_tronco(fd_tronchi[i][1],npc[i]);
        }
        close(fd_tronchi[i][1]);//blocca la scrittura nella pipe per il processo padre
    }
    
}

void gestione_tronco(int fd_tronco, oggetto_tronco npc_tronco)
{ 
    //caso partanza da destra con dimensione x2
    if(npc_tronco.verso == -1 && npc_tronco.id_sprite == ID_TRUNK_X2)
    {
        int direzione = -L_FROGGER;
        npc_tronco.verso = -1;

        while (true)
        {
            write(fd_tronco, &direzione, sizeof(direzione));//scrittura posizione nella processo_tronco
            npc_tronco.x += direzione;

            if(npc_tronco.x + L_TRUNK_X2 >= MAXX - 30) //cambia la direzione del tronco se tocca il bordo destro
            {
                direzione *= -1;
            }
            else if(npc_tronco.x <= 30)//cambia la direzione del tronco se tocca il bordo sinistro
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
    else if(npc_tronco.verso == 1 && npc_tronco.id_sprite == ID_TRUNK_X2)
    {
        int direzione = +L_FROGGER;
        npc_tronco.verso= 1;

        while (true)
        {
            write(fd_tronco, &direzione, sizeof(direzione));//scrittura posizione nella processo_tronco
            npc_tronco.x += direzione;
            if(npc_tronco.x + L_TRUNK_X2 >= MAXX -30) //cambia la direzione del tronco se tocca il bordo destro
            {
                direzione *= -1;
            }
            else if(npc_tronco.x <= 30)//cambia la direzione del tronco se tocca il bordo sinistro
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
    else if(npc_tronco.verso == -1 && npc_tronco.id_sprite == ID_TRUNK_X3)
    {
        int direzione = -L_FROGGER;
        npc_tronco.verso= -1;

        while(true)
        {
            write(fd_tronco, &direzione, sizeof(direzione));//scrittura posizione nella processo_tronco
            npc_tronco.x += direzione;
            if(npc_tronco.x + L_TRUNK_X3 >= MAXX -30) //cambia la direzione del tronco se tocca il bordo destro
            {
                direzione *= -1;
            }
            else if(npc_tronco.x <= 30)//cambia la direzione del tronco se tocca il bordo sinistro
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
    else if(npc_tronco.verso == 1 && npc_tronco.id_sprite == ID_TRUNK_X3)
    {
        int direzione = +L_FROGGER;
        npc_tronco.verso = +1;

        while (true)
        {
            write(fd_tronco, &direzione, sizeof(direzione));//scrittura posizione nella processo_tronco
            npc_tronco.x += direzione;
            
            if(npc_tronco.x + L_TRUNK_X3 >= MAXX - 30) //cambia la direzione del tronco se tocca il bordo destro
            {
                direzione *= -1;
            }
            else if(npc_tronco.x <= 30)//cambia la direzione del tronco se tocca il bordo sinistro
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
    int spostamento;
    for (size_t i = 0; i < N_CORSIE_FIUME; i++)
        {
            read(fd_tronco[i][0],&spostamento,sizeof(spostamento));     //leggo il tronco
            tronchi[i].x += spostamento;        //aggiorno la x del tronco
        }
}