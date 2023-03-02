#include "gestione_grafica.h"
#include "fiume.h"
#include "npc.h"
#include "strada.h"
#include "collisioni.h"

void frogger(int fd_rana, int fd_tronco[N_CORSIE_FIUME][2], int fd_trasporto_rana,int fd_veicolo[N_VEICOLI][2], int fd_proiettili_alleati [MAX_PROIETTILI][2]);

WINDOW *wtime;

int main()
{
    wtime=newwin(4,10,MAXY+4, 0);
    box(wtime, ACS_VLINE, ACS_HLINE);
    
    pid_t p_rana, p_tronco[N_CORSIE_FIUME], p_time,p_veicoli[N_VEICOLI];
    int fd_rana[2];
    int fd_tronchi[N_CORSIE_FIUME][2];
    int fd_trasporto_rana[2];
    int fd_veicolo [N_VEICOLI][2];
    int fd_proiettili_alleati [MAX_PROIETTILI][2];

    //crea la finestra e attiva/disattiva i comandi richiesti
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, true);
    start_color();
    curs_set(0);

    generazione_processi_tronco(fd_tronchi,p_tronco);

    genera_processi_veicoli(fd_veicolo,p_veicoli);

    CHECK_PIPE(fd_rana);//verifica se la pipe e' stata creata correttamente
    CHECK_PIPE(fd_trasporto_rana);
    CHECK_PID(p_rana);//creo processo figlio rana
    fcntl(fd_rana[0],F_SETFL, O_NONBLOCK);
    fcntl(fd_trasporto_rana[0],F_SETFL, O_NONBLOCK);

    if(!p_rana)
    {
        close(fd_trasporto_rana[1]);
        wasd_rana(fd_rana[1],fd_trasporto_rana[0]);
        clear();
    }
    else
    {
        CHECK_PID(p_time);

        if(!p_time)
        {
            funzione_tempo_gioco(wtime);
        }
        else
        {
            close(fd_rana[1]); /* chiusura del descrittore di scrittura */
            for (int i = 0; i < N_CORSIE_FIUME; i++)
                close(fd_tronchi[i][1]);
            close(fd_trasporto_rana[0]);

            frogger(fd_rana[0], fd_tronchi, fd_trasporto_rana[1],fd_veicolo,fd_proiettili_alleati);
        }
    }
    
    kill(p_rana,1);
    kill(p_time,1);
    for(int i=0; i<N_CORSIE_FIUME;i++)
        kill(p_tronco[i],1);
    for(int i=0; i<N_VEICOLI;i++)
        kill(p_veicoli[i],1);
    
    endwin();
}

void frogger(int fd_rana, int fd_tronco[N_CORSIE_FIUME][2], int fd_trasporto_rana,int fd_veicolo[N_VEICOLI][2], int fd_proiettili_alleati [MAX_PROIETTILI][2])
{
    oggetto_rana npc_rana = {0,0,0,false,0};
    oggetto_tronco tronchi[N_CORSIE_FIUME]; 
    oggetto_veicolo veicolo[N_VEICOLI];
    oggetto_proiettile proiettile_alleato [MAX_PROIETTILI];
    init_pair(5,COLOR_WHITE,COLOR_BLUE); //tronco fiume
    int collisione=0;
    int hitproiettile =0;
    int time=0;
    int Nproiettile=0;
    
    while(true)
    {
        clear();
        mappa_gioco();

        for(int i =0;i<N_VEICOLI;i++)
        {
            close(fd_veicolo[i][1]);
            read(fd_veicolo[i][0],&veicolo[i],sizeof(oggetto_veicolo));
        }
    
        lettura_pipe_tronchi(tronchi, fd_tronco);

        read(fd_rana,&npc_rana,sizeof(oggetto_rana));

        if(npc_rana.y > 7 && npc_rana.y < 29)
        {
            tronco_taxi(&npc_rana, tronchi[(npc_rana.y/H_FROGGER)-2]);
            write(fd_trasporto_rana, &npc_rana, sizeof(oggetto_rana));
            read(fd_rana,&npc_rana,sizeof(oggetto_rana));
            attron(COLOR_PAIR(10));
            mvprintw(0,0,"X: %d Y: %d",npc_rana.x, npc_rana.y);
            mvprintw(1,0,"SENTIELLA: %d VERSO: %d",npc_rana.sentinella_trasporto, npc_rana.verso);
        }

        if (npc_rana.sparo == 1)
        {
            if (Nproiettile == MAX_PROIETTILI)
                Nproiettile =0;
            proiettile_alleato[Nproiettile].pid= genera_processo_proiettile(fd_proiettili_alleati[Nproiettile],npc_rana);
            proiettile_alleato[Nproiettile].sparato =1;
            
            Nproiettile++;
            npc_rana.sparo =0;
        }

        for(int i =0;i < MAX_PROIETTILI;i++)
        {
            if(proiettile_alleato[i].sparato == 1)
            {
                close(fd_proiettili_alleati[i][1]);
                aggiorna_proiettile(fd_proiettili_alleati[i][0],&proiettile_alleato[i]);
            }
        }

        //hitproiettile = collisioni_proiettile();

        collisione = collisioni_rana_veicoli(npc_rana,veicolo);

        stampa_tronchi(tronchi);

        stampa_proiettili(proiettile_alleato);

        init_pair(10,COLOR_WHITE,calcola_background(npc_rana.x,npc_rana.y));
        attron(COLOR_PAIR(10));
        print_sprite(npc_rana.x, npc_rana.y, FROGGER, DIM_FROGGER);

        stampa_veicoli(veicolo);
        refresh();
    }  
    
}