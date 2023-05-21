#include "Collisioni.h"

void* tempoDiGioco()
{
    while (running)
    {
        sleep(1);
        max_time--;
    }
}

void inizializza_proiettili()
{
    for (size_t i = 0; i < N_MAX_P; i++)
    {
        proiettili_alleati[i].x = -1;
        proiettili_alleati[i].verso_proiettile = -1;

        proiettili_nemici[i].x = -1;
        proiettili_nemici[i].verso_proiettile = 1;
    }   
}

void game_over()
{
    if(max_time == 0 || vite<1 || vittoria())
            running = false;//fine partita 
}

void collisioni_rana_veicoli()
{
    for (int i=0;i< N_VEICOLI;i++)
    {
        if (veicolo[i].id_sprite == 1 || veicolo[i].id_sprite == 4) //hitbox macchina
        {
            for(int j=0;j<14;j++)
            {
                if ((player.x == veicolo[i].x + j && player.y == veicolo[i].y) || (player.x + 8 == veicolo[i].x + j && player.y == veicolo[i].y))
                {
                    //pthread_mutex_lock (&semCurses);
                    vite--;
                    finemanche = 6;
                    player.x = X_START;
                    player.y = Y_START;
                    //pthread_mutex_unlock (&semCurses);
                }
            }
        }else //hitbox camion/pullman
        {
            for(int j=0;j<28;j++)
            {
                if ((player.x == veicolo[i].x + j && player.y == veicolo[i].y) || (player.x + 8 == veicolo[i].x + j && player.y == veicolo[i].y))
                {
                    //pthread_mutex_lock (&semCurses);
                    vite--;
                    finemanche = 6;
                    player.x = X_START;
                    player.y = Y_START;
                    //pthread_mutex_unlock (&semCurses);
                }
            }
        }
    }
}

void* gestione_proiettili(void* Proiettile)
{
    oggetto_proiettile* P = (oggetto_proiettile*) Proiettile;

    int movimento = 1;

    while(running)
    {
        if (P->x != -1)
        {
            pthread_mutex_lock (&semCurses);
            P->y += (movimento * P->verso_proiettile);
            pthread_mutex_unlock (&semCurses);
        }
        usleep(TIME_MAIN);
    }
}

void collisioni_proiettili_bordi()
{

    for (size_t i = 0; i < N_MAX_P; i++)
    {
        if (proiettili_alleati[i].y < 0 && proiettili_alleati[i].x != -1 )
        {
            proiettili_alleati[i].x = -1;
        }
    }

    for (size_t i = 0; i < N_MAX_P; i++)
    {
        if (proiettili_nemici[i].y > (Y_START + H_FROGGER -2) && proiettili_nemici[i].x != -1 )
        {
            proiettili_nemici[i].x = -1;
        }
    }
}

void collisioni_proiettili_macchine()
{
    for (size_t i = 0; i < N_VEICOLI; i++)
    {
        // for (size_t l = 0; l < N_MAX_P; l++) // proiettili alleati
        // {
        //      if (veicolo[i].id_sprite == 1 || veicolo[i].id_sprite == 4) //hitbox macchina
        //     {
        //         for(int j=0;j<14;j++)
        //         {
        //             if (proiettili_alleati[l].x == veicolo[i].x + j && proiettili_alleati[l].y == veicolo[i].y + 3 )
        //             {
        //                 return 1;
        //             }else
        //             {
        //                 for (size_t m = 0; m < 3; m++)
        //                 {
        //                     if ((proiettili_alleati[l].x == veicolo[i].x && proiettili_alleati[l].y == veicolo[i].y + m)||(proiettili_alleati[l].x == veicolo[i].x + 13 && proiettili_alleati[l].y == veicolo[i].y + m))
        //                     {
        //                         return 1;
        //                     }
        //                 }
                        
        //             }
        //         }
        //     }else //hitbox camion/pullman
        //     {
        //         for(int j=0;j<28;j++)
        //         {
        //             if (proiettili_alleati[l].x == veicolo[i].x + j && proiettili_alleati[l].y == veicolo[i].y + 3 )
        //             {
        //                 return 1;
        //             }else
        //             {
        //                 for (size_t m = 0; m < 3; m++)
        //                 {
        //                     if ((proiettili_alleati[l].x == veicolo[i].x && proiettili_alleati[l].y == veicolo[i].y + m)||(proiettili_alleati[l].x == veicolo[i].x + 13 && proiettili_alleati[l].y == veicolo[i].y + m))
        //                     {
        //                         return 1;
        //                     }
        //                 }
                        
        //             }
        //         }
        //     }
        // }
        for (size_t l = 0; l < N_MAX_P; l++) //proiettili nemici
        {
            if (veicolo[i].id_sprite == 1 || veicolo[i].id_sprite == 4) //hitbox macchina
            {
                for(int j=0;j<14;j++)
                {
                    if (proiettili_nemici[l].x == veicolo[i].x + j && proiettili_nemici[l].y == veicolo[i].y)
                    {
                        proiettili_nemici[l].x = -1;
                    }else
                    {
                        for (size_t m = 1; m < 4; m++)
                        {
                            if ((proiettili_nemici[l].x == veicolo[i].x && proiettili_nemici[l].y == veicolo[i].y + m)||(proiettili_nemici[l].x == veicolo[i].x + 13 && proiettili_nemici[l].y == veicolo[i].y + m))
                            {
                                proiettili_nemici[l].x = -1;
                            }
                        }
                        
                    }
                }
            }else //hitbox camion/pullman
            {
                for(int j=0;j<28;j++)
                {
                    if (proiettili_nemici[l].x == veicolo[i].x + j && proiettili_nemici[l].y == veicolo[i].y)
                    {
                        proiettili_nemici[l].x = -1;
                    }else
                    {
                        for (size_t m = 1; m < 4; m++)
                        {
                            if ((proiettili_nemici[l].x == veicolo[i].x && proiettili_nemici[l].y == veicolo[i].y + m)||(proiettili_nemici[l].x == veicolo[i].x + 27 && proiettili_nemici[l].y == veicolo[i].y + m))
                            {
                                proiettili_nemici[l].x = -1;
                            }
                        }
                        
                    }
                }
            }
        }
        
    }
    
}

void stampa_proiettili()
{
    for (size_t i = 0; i < N_MAX_P; i++)
    {
        if(proiettili_alleati[i].x != -1)
        {
            init_pair(PROIETTILE + i,COLOR_RED, calcola_background(proiettili_alleati[i].x, proiettili_alleati[i].y));
            wattron(win_mappa, COLOR_PAIR(PROIETTILE + i));
            mvwprintw(win_mappa, proiettili_alleati[i].y, proiettili_alleati[i].x, "^");
            wattroff(win_mappa,COLOR_PAIR(PROIETTILE + i));
        }
        if(proiettili_nemici[i].x != -1){
            init_pair(PROIETTILE + N_MAX_P + i,COLOR_RED, calcola_background(proiettili_nemici[i].x, proiettili_nemici[i].y)); 
            wattron(win_mappa, COLOR_PAIR(PROIETTILE + N_MAX_P + i));
            mvwprintw(win_mappa, proiettili_nemici[i].y, proiettili_nemici[i].x, "v");
            wattroff(win_mappa,COLOR_PAIR(PROIETTILE + N_MAX_P + i));
        }
    }
}

void collisioni_proiettiliA_proiettiliN()
{
    for (size_t i = 0; i < N_MAX_P; i++)
    {
        if(proiettili_alleati[i].x != -1){
            for (size_t j = 0; j < N_MAX_P; j++)
            {
               if(proiettili_nemici[j].x != -1){
                    if((proiettili_alleati[i].x == proiettili_nemici[j].x) && ((proiettili_alleati[i].y - proiettili_nemici[j].y) < 2)){
                        proiettili_nemici[j].x = -1;
                        proiettili_alleati[i].x = -1;
                    }
               }
            }
        }
    }
}

void collisioni_proiettile_enemy()
{
    for (size_t i = 0; i < N_MAX_ENEMY; i++)
    {
        if(enemy[i].x != -1){
            for (size_t j = 0; j < N_MAX_P; j++)
            {
                if((proiettili_alleati[j].x == enemy[i].x +H_FROGGER) && (proiettili_alleati[j].y >= enemy[i].y && proiettili_alleati[j].y <= enemy[i].y +H_FROGGER ) )
                {
                    enemy[i].x = -1;
                    proiettili_alleati[j].x = -1;
                }
            }
        }
    }  
}

void collisione_player_proiettileN()
{
    for (size_t i = 0; i < N_MAX_ENEMY; i++)
    {
        if(proiettili_nemici[i].x!= -1){
            if((proiettili_nemici[i].x == player.x +H_FROGGER) && (proiettili_nemici[i].y >= player.y && proiettili_nemici[i].y <= player.y +H_FROGGER ))
            {
                player.x = X_START;
                player.y = Y_START;
                proiettili_nemici[i].x = -1;
                vite--;
            }
        }
    }
}

void collisione_player_enemy()
{
    for (size_t i = 0; i < N_MAX_ENEMY; i++)
    {
        if(enemy[i].x!= -1){
            if(enemy[i].y == player.y)
            {
                player.x = X_START;
                player.y = Y_START;
                vite--;
            }
        }
    }
}

void collisioni_game()
{
    collisioni_proiettili_macchine();
    collisioni_proiettiliA_proiettiliN();
    collisione_player_enemy();
    collisioni_rana_veicoli();
    collisioni_proiettili_bordi();
    collisioni_proiettile_enemy();
    collisione_player_proiettileN();
}
