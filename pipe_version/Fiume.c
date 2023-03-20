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
    srand(getpid()); //inizializzo la random dando come seme il PID del processo
    bool traslazione_iniziale = true; //abilita/disabilita la traslazine iniziale

    //caso partanza da destra con dimensione x2
    if(npc_tronco.verso == -1 && npc_tronco.id_sprite == ID_TRUNK_X2)
    {
        int direzione = -L_FROGGER;

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

            usleep(TIME);
        } 
    }

    //caso patenza sinistra con dimensione x2
    else if(npc_tronco.verso == 1 && npc_tronco.id_sprite == ID_TRUNK_X2)
    {
        int direzione = +L_FROGGER;

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
            
            usleep(TIME);
        }
    }
    
    //caso partanza da destra con dimensione x3
    else if(npc_tronco.verso == -1 && npc_tronco.id_sprite == ID_TRUNK_X3)
    {
        int direzione = -L_FROGGER;

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

            usleep(TIME);
        }
    }

    //caso patenza sinistra con dimensione x3
    else if(npc_tronco.verso == 1 && npc_tronco.id_sprite == ID_TRUNK_X3)
    {
        int direzione = +L_FROGGER;
        int fattore_traslazione= L_FROGGER;

        fattore_traslazione += L_FROGGER * rand()%2;

        while (true)
        {
            if(!traslazione_iniziale){
                write(fd_tronco, &direzione, sizeof(direzione));//scrittura posizione nella processo_tronco
                npc_tronco.x += direzione;
            }
            else{
                write(fd_tronco, &fattore_traslazione, sizeof(fattore_traslazione));//scrittura posizione nella processo_tronco
                traslazione_iniziale = false;
                npc_tronco.x += fattore_traslazione;
            }
    
            if(npc_tronco.x + L_TRUNK_X3 >= MAXX - 30) //cambia la direzione del tronco se tocca il bordo destro
            {
                direzione *= -1;
            }
            else if(npc_tronco.x <= 30)//cambia la direzione del tronco se tocca il bordo sinistro
            {
                direzione *= -1;
            }
    
            usleep(TIME);
        }
    }
}

void lettura_pipe_tronchi(oggetto_rana *npc_rana, oggetto_tronco tronchi[N_CORSIE_FIUME], int fd_tronco[N_CORSIE_FIUME][2], int *vite)
{
    int spostamento;
    for (size_t i = 0; i < N_CORSIE_FIUME; i++)
        {
            read(fd_tronco[i][0],&spostamento,sizeof(spostamento));     //leggo il tronco
            if(tronco_taxi(npc_rana, tronchi[i], spostamento))     //se la rana è sopra il tronco segue il suo movimento
                (*vite)--;   //la rana è caduta nel fiume
            tronchi[i].x += spostamento;       //aggiorno la x del tronco
        }
}

int tronco_taxi(oggetto_rana *npc_rana, oggetto_tronco npc_tronco, int spostamento)
{ 
    if(npc_rana->y == npc_tronco.y){
        if((npc_rana->x == npc_tronco.x)){
            if(abilita_movimento_confini_mappa(*npc_rana, spostamento))
                npc_rana->x += spostamento;
            return 0;
        }
        else if((npc_rana->x == npc_tronco.x + L_FROGGER)){
            if(abilita_movimento_confini_mappa(*npc_rana, spostamento))
                npc_rana->x += spostamento;
            return 0;
        }
        else if((npc_tronco.id_sprite == ID_TRUNK_X3) && (npc_rana->x == npc_tronco.x + 2*L_FROGGER)){
            if(abilita_movimento_confini_mappa(*npc_rana, spostamento))
                npc_rana->x += spostamento;
            return 0;
        }
        else{                         
            //se la rana cade nel fiume torna alla posizione di partenza
            npc_rana->x = X_START;
            npc_rana->y = Y_START;
            return 1;
        }
    }
    else{
        return 0;
    }
}

void inizializza_posizione_tane(oggetto_tana tane[N_TANE])
{
    int indice_cella=0;
    for (size_t i = 39; i < MAXX -30; i+=18)
    {
        tane[indice_cella].x = i;
        tane[indice_cella].y = MAX_PRATO;
        tane[indice_cella].id_sprite = ID_TANA;
        tane[indice_cella].occupata = false;
        indice_cella++;   
    }
    
}

int tana_occupata(oggetto_rana * player, oggetto_tana tane[N_TANE])
{   
    if(player->y == MAX_PRATO && !((player->x / L_FROGGER) % 2)){
        for (size_t i = 0; i < N_TANE; i++)
        {
            if(tane[i].x == player->x)
                tane[i].occupata = true;
        }

        player->x = X_START;
        player->y = Y_START;

        return 1;
    }
    return 0;
}

void print_tane_occupate(oggetto_tana tana[N_TANE])
{
    init_color(100, 0, 128, 0); // definizione del nuovo colore verde scuro
    init_pair(TANA_OCCUPATA, 100, COLOR_BLUE); // palette simbolo tana occupata con il nuovo colore verde scuro
    for (size_t i = 0; i < N_TANE; i++)
    {
        if(tana[i].occupata){
            wattron(win_mappa,COLOR_PAIR(TANA_OCCUPATA));
            print_sprite(tana[i].x, tana[i].y,TANA);
        }
    }
}

int check_tana(int x, int y)
{
    for (size_t i = 0; i < N_TANE; i++)
    {
        if(tane_gioco[i].x == x && (tane_gioco[i].y + H_FROGGER) == y  && tane_gioco[i].occupata)
            return 1;
    }
    return 0;
}