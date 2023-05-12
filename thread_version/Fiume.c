#include "Fiume.h"
#define COLORE_MARRONE 8

void inizializza_tronchi()
{
    srand(time(NULL));//inizializza il seme per la rand

    int verso, dimensione_tronco;
    int traslazioni[N_CORSIE_FIUME];

    //calcolo randomicamente una traslazione iniziale per avere movimenti diversi dei tronchi
    for (int i = 0; i < N_CORSIE_FIUME; i++) {
        traslazioni[i] = (L_FROGGER*(rand()%4));  // genero l'incremento casuale [set casuale-->(0,9,18,27)]
        for (int j = 0; j < i; j++) {
            if (traslazioni[j] == traslazioni[i]) {  // controllo se l'incremento è gia stato generato
                i--;  // se sì, decrementa i per ripetere il ciclo e generarne un'altro
                break;
            }
        }
    }
    //inizializzo le strutture tronchi per avere la stessa coppia sia nel padre sia nel figlio
    for (int i = 0; i < N_CORSIE_FIUME; i++)
    {
        //determino randomicamente il verso del tronco ( 0 -. destra | 1 -. sinistra)
        verso = rand()%2;
        //determino randomicamente la dimensione del tronco (0 -. x2 | 1 -. x3)
        dimensione_tronco = rand()%2;
        //inizializza la struttura specifica
        set_tronco(i, verso, dimensione_tronco, traslazioni[i]);
    }
}

void set_tronco(int corsia, int verso, int dimensione_tronco, int traslazione_iniziale)
{    
    //caso partanza da destra con dimensione x2
    if(!verso && !dimensione_tronco)
    {
        //posiziona il tronco nelle rispettive cordinate iniziali della rispettiva corsia
        tronchi[corsia].x = (MAXX-30) - (L_TRUNK_X2 + traslazione_iniziale);
        tronchi[corsia].y =H_FROGGER * (2 + corsia);
        tronchi[corsia].id_sprite = ID_TRUNK_X2;
        tronchi[corsia].corsia = corsia;
        tronchi[corsia].verso = -1;
    }

    //caso patenza sinistra con dimensione x2
    else if(verso && !dimensione_tronco)
    {
        //posiziona il tronco nelle rispettive cordinate iniziali della rispettiva corsia
        tronchi[corsia].x = 30 + traslazione_iniziale;
        tronchi[corsia].y =H_FROGGER * (2 + corsia);
        tronchi[corsia].id_sprite = ID_TRUNK_X2;
        tronchi[corsia].corsia = corsia;
        tronchi[corsia].verso= +1;
    }
    
    //caso partanza da destra con dimensione x3
    else if(!verso && dimensione_tronco)
    {
        //posiziona il tronco nelle rispettive cordinate iniziali della rispettiva corsia
        tronchi[corsia].x = (MAXX-30) - (L_TRUNK_X3 + traslazione_iniziale);
        tronchi[corsia].y =H_FROGGER * (2 + corsia);
        tronchi[corsia].id_sprite = ID_TRUNK_X3;
        tronchi[corsia].corsia = corsia;
        tronchi[corsia].verso= -1;
    }

    //caso patenza sinistra con dimensione x3
    else if(verso && dimensione_tronco)
    {
        //posiziona il tronco nelle rispettive cordinate iniziali della rispettiva corsia
        tronchi[corsia].x = 30 + traslazione_iniziale;
        tronchi[corsia].y =H_FROGGER * (2 + corsia);
        tronchi[corsia].id_sprite = ID_TRUNK_X3;
        tronchi[corsia].corsia = corsia;
        tronchi[corsia].verso = +1;
    }
}

void* gestione_tronco(void* tronco)
{ 
    oggetto_tronco* npc_tronco = (oggetto_tronco *) tronco;
    //caso partanza da destra con dimensione x2
    if(npc_tronco->verso == -1 && npc_tronco->id_sprite == ID_TRUNK_X2)
    {
        int direzione = -L_FROGGER;

        while (true)
        {
            pthread_mutex_lock (&semCurses);
             if(tronco_taxi(npc_tronco->corsia,direzione))      //se la rana è sopra il tronco segue il suo movimento
                vite--;   //la rana è caduta nel fiume
                
            npc_tronco->x += direzione;           
                
            pthread_mutex_unlock (&semCurses);

            if(npc_tronco->x + L_TRUNK_X2 >= MAXX - 30) //cambia la direzione del tronco se tocca il bordo destro
            {
                direzione *= -1;
            }
            else if(npc_tronco->x <= 30)//cambia la direzione del tronco se tocca il bordo sinistro
            {
                direzione *= -1;
            }

            usleep(TIME);
        } 
    }

    //caso patenza sinistra con dimensione x2
    else if(npc_tronco->verso == 1 && npc_tronco->id_sprite == ID_TRUNK_X2)
    {
        int direzione = +L_FROGGER;

        while (true)
        {
            pthread_mutex_lock (&semCurses);
            

            if(tronco_taxi(npc_tronco->corsia,direzione))      //se la rana è sopra il tronco segue il suo movimento
                vite--;   //la rana è caduta nel fiume
            npc_tronco->x += direzione;

            pthread_mutex_unlock (&semCurses);

            if(npc_tronco->x + L_TRUNK_X2 >= MAXX -30) //cambia la direzione del tronco se tocca il bordo destro
            {
                direzione *= -1;
            }
            else if(npc_tronco->x <= 30)//cambia la direzione del tronco se tocca il bordo sinistro
            {
                direzione *= -1;
            }
            
            usleep(TIME);
        }
    }
    
    //caso partanza da destra con dimensione x3
    else if(npc_tronco->verso == -1 && npc_tronco->id_sprite == ID_TRUNK_X3)
    {
        int direzione = -L_FROGGER;

        while(true)
        {
            pthread_mutex_lock (&semCurses);

            if(tronco_taxi(npc_tronco->corsia,direzione))      //se la rana è sopra il tronco segue il suo movimento
                vite--;   //la rana è caduta nel fiume

            npc_tronco->x += direzione;
            pthread_mutex_unlock (&semCurses);

            if(npc_tronco->x + L_TRUNK_X3 >= MAXX -30) //cambia la direzione del tronco se tocca il bordo destro
            {
                direzione *= -1;
            }
            else if(npc_tronco->x <= 30)//cambia la direzione del tronco se tocca il bordo sinistro
            {
                direzione *= -1;
            }

            usleep(TIME);
        }
    }

    //caso patenza sinistra con dimensione x3
    else if(npc_tronco->verso == 1 && npc_tronco->id_sprite == ID_TRUNK_X3)
    {
        int direzione = +L_FROGGER;

        while (true)
        {
            pthread_mutex_lock (&semCurses);

            if(tronco_taxi(npc_tronco->corsia,direzione))     //se la rana è sopra il tronco segue il suo movimento
                vite--;   //la rana è caduta nel fiume

            npc_tronco->x += direzione;    
            pthread_mutex_unlock (&semCurses);

            if(npc_tronco->x + L_TRUNK_X3 >= MAXX - 30) //cambia la direzione del tronco se tocca il bordo destro
            {
                direzione *= -1;
            }
            else if(npc_tronco->x <= 30)//cambia la direzione del tronco se tocca il bordo sinistro
            {
                direzione *= -1;
            }
    
            usleep(TIME);
        }
    }
}

void stampa_tronchi ()
{
    // definisci un nuovo colore personalizzato per il marrone
    init_color(COLORE_MARRONE,139, 69, 19); // Inizializza il colore 8 come RGB (100, 200, 300)

    init_pair(TRONCO,COLORE_MARRONE,COLOR_BLUE); //palette tronco

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

int tronco_taxi(int corsia, int spostamento)
{ 
    if(player.y == tronchi[corsia].y){
        if((player.x == tronchi[corsia].x)){
            if(abilita_movimento_confini_mappa(spostamento))
                player.x += spostamento;
            return 0;
        }
        else if((player.x == tronchi[corsia].x + L_FROGGER)){
            if(abilita_movimento_confini_mappa(spostamento))
                player.x += spostamento;
            return 0;
        }
        else if((tronchi[corsia].id_sprite == ID_TRUNK_X3) && (player.x == tronchi[corsia].x + 2*L_FROGGER)){
            if(abilita_movimento_confini_mappa(spostamento))
                player.x += spostamento;
            return 0;
        }
        else{                         
            //se la rana cade nel fiume torna alla posizione di partenza
            player.x = X_START;
            player.y = Y_START;
            return 1;
        }
    }
    
    return 0;
}