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

        while (running)
        {
            pthread_mutex_lock (&semCurses);
            if(tronco_taxi(npc_tronco->corsia,direzione)){     //se la rana è sopra il tronco segue il suo movimento
                vite--;   //la rana è caduta nel fiume
                max_time = 60;
                finemanche = 6;
            }

            for (size_t i = 0; i < N_MAX_ENEMY; i++)
            {
                if(enemy[i].x != -1 && enemy[i].y == npc_tronco->y)
                    enemy[i].x += direzione; 
            }
                
            npc_tronco->x += direzione;           
                
            pthread_mutex_unlock (&semCurses);

            if(npc_tronco->x + L_TRUNK_X2 >= MAXX - 30) //cambia la direzione del tronco se tocca il bordo destro
            {
                direzione *= -1;
                npc_tronco->verso *= -1;
            }
            else if(npc_tronco->x <= 30)//cambia la direzione del tronco se tocca il bordo sinistro
            {
                direzione *= -1;
                npc_tronco->verso *= -1;
            }

            usleep(TIME);
        } 
    }

    //caso patenza sinistra con dimensione x2
    else if(npc_tronco->verso == 1 && npc_tronco->id_sprite == ID_TRUNK_X2)
    {
        int direzione = +L_FROGGER;

        while (running)
        {
            pthread_mutex_lock (&semCurses);
            

            if(tronco_taxi(npc_tronco->corsia,direzione)){     //se la rana è sopra il tronco segue il suo movimento
                vite--;   //la rana è caduta nel fiume
                finemanche = 6;
            }

            for (size_t i = 0; i < N_MAX_ENEMY; i++)
            {
                if(enemy[i].x != -1 && enemy[i].y == npc_tronco->y)
                    enemy[i].x += direzione; 
            }

            npc_tronco->x += direzione;

            pthread_mutex_unlock (&semCurses);

            if(npc_tronco->x + L_TRUNK_X2 >= MAXX -30) //cambia la direzione del tronco se tocca il bordo destro
            {
                direzione *= -1;
                npc_tronco->verso *= -1;
            }
            else if(npc_tronco->x <= 30)//cambia la direzione del tronco se tocca il bordo sinistro
            {
                direzione *= -1;
                npc_tronco->verso *= -1;
            }
            
            usleep(TIME);
        }
    }
    
    //caso partanza da destra con dimensione x3
    else if(npc_tronco->verso == -1 && npc_tronco->id_sprite == ID_TRUNK_X3)
    {
        int direzione = -L_FROGGER;

        while(running)
        {
            pthread_mutex_lock (&semCurses);

            if(tronco_taxi(npc_tronco->corsia,direzione)){     //se la rana è sopra il tronco segue il suo movimento
                vite--;   //la rana è caduta nel fiume
                finemanche = 6;
            }

            for (size_t i = 0; i < N_MAX_ENEMY; i++)
            {
                if(enemy[i].x != -1 && enemy[i].y == npc_tronco->y)
                    enemy[i].x += direzione; 
            }

            npc_tronco->x += direzione;
            pthread_mutex_unlock (&semCurses);

            if(npc_tronco->x + L_TRUNK_X3 >= MAXX -30) //cambia la direzione del tronco se tocca il bordo destro
            {
                direzione *= -1;
                npc_tronco->verso *= -1;
            }
            else if(npc_tronco->x <= 30)//cambia la direzione del tronco se tocca il bordo sinistro
            {
                direzione *= -1;
                npc_tronco->verso *= -1;
            }

            usleep(TIME);
        }
    }

    //caso patenza sinistra con dimensione x3
    else if(npc_tronco->verso == 1 && npc_tronco->id_sprite == ID_TRUNK_X3)
    {
        int direzione = +L_FROGGER;

        while (running)
        {
            pthread_mutex_lock (&semCurses);

            for (size_t i = 0; i < N_MAX_ENEMY; i++)
            {
                if(enemy[i].x != -1 && enemy[i].y == npc_tronco->y)
                    enemy[i].x += direzione; 
            }
            
            if(tronco_taxi(npc_tronco->corsia,direzione)){     //se la rana è sopra il tronco segue il suo movimento
                vite--;   //la rana è caduta nel fiume
                finemanche = 6;
            }
                

            npc_tronco->x += direzione;    
            pthread_mutex_unlock (&semCurses);

            if(npc_tronco->x + L_TRUNK_X3 >= MAXX - 30) //cambia la direzione del tronco se tocca il bordo destro
            {
                direzione *= -1;
                npc_tronco->verso *= -1;
            }
            else if(npc_tronco->x <= 30)//cambia la direzione del tronco se tocca il bordo sinistro
            {
                direzione *= -1;
                npc_tronco->verso *= -1;
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

void inizializza_posizione_tane()
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

int tana_occupata()
{   
    if(player.y == MAX_PRATO && !((player.x / L_FROGGER) % 2)){
        for (size_t i = 0; i < N_TANE; i++)
        {
            if(tane[i].x == player.x)
                tane[i].occupata = true;
        }

        player.x = X_START;
        player.y = Y_START;

        return 1;
    }
    return 0;
}

void fuori_area_tane ()
{
    if(player.y == MAX_PRATO && ((player.x / L_FROGGER) % 2))
        {
            vite--; //se in altezza tana non è stata occupata una tana decrementa la vita di 1 (è uscito fuori dalla)
            player.x = X_START;
            player.y = Y_START;
            finemanche =6;
        }
}

void print_tane_occupate()
{
    init_color(100, 0, 128, 0); // definizione del nuovo colore verde scuro
    init_pair(TANA_OCCUPATA, 100, COLOR_BLUE); // palette simbolo tana occupata con il nuovo colore verde scuro
    for (size_t i = 0; i < N_TANE; i++)
    {
        if(tane[i].occupata){
            wattron(win_mappa,COLOR_PAIR(TANA_OCCUPATA));
            print_sprite(tane[i].x, tane[i].y,TANA);
        }
    }
}

int check_tana(int x, int y)
{
    for (size_t i = 0; i < N_TANE; i++)
    {
        if(tane[i].x == x && (tane[i].y + H_FROGGER) == y  && tane[i].occupata)
            return 1;
    }
    return 0;
}

void collisioni_tane_occupate()
{
    for (size_t i = 0; i < N_TANE; i++)
    {
        if ((player.x == tane[i].x) &&(player.y == tane[i].y) && tane[i].occupata){
            player.x = X_START;
            player.y = Y_START;
            vite--;
            finemanche=6;
        }
    }
}