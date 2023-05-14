#include "NPC.h"

#define SPOSTA_SU -4
#define SPOSTA_GIU +4
#define SPOSTA_DESTRA +9
#define SPOSTA_SINISTRA -9
#define SPACE 32

#define TIME_SPARO_MIN 1000000
#define RANGE_TIME_SPARO 2500000

void* wasd_rana()
{
    int direzione;
    int sparato = 1;
    keypad(win_mappa, true);
    nodelay(win_mappa, false);
    
    //inizializzo le coordinate rana
    player.x = X_START;
    player.y = Y_START;
    player.id_sprite = ID_FROGGER;

    while (running)
    {
        direzione = wgetch(win_mappa);
        pthread_mutex_lock (&semCurses);

        switch (direzione)
        {
        case KEY_UP:
            if(abilita_movimento_confini_mappa(SPOSTA_SU))
                player.y -= H_FROGGER;
            break;
        case KEY_DOWN:
            if(abilita_movimento_confini_mappa(SPOSTA_GIU))
                player.y += H_FROGGER;
            break;
        case KEY_LEFT:
            if(abilita_movimento_confini_mappa(SPOSTA_SINISTRA))
                player.x -= L_FROGGER;
            break;
        case KEY_RIGHT:
            if(abilita_movimento_confini_mappa(SPOSTA_DESTRA))
                player.x += L_FROGGER;
            break;
        case SPACE:
            for (int i = 0; i < N_MAX_P; i++)
            {
                if(proiettili_alleati[i].x  == -1){
                    proiettili_alleati[i].x = player.x + 4;
                    proiettili_alleati[i].y = player.y -1;
                
                    i = N_MAX_P;
                } 
            }
            break;
        }
        pthread_mutex_unlock (&semCurses);
    }
}

void genera_enemy()
{
    int tronco_idx;
    int traslazione;

        srand(time(NULL)); 
    
        if((rand()%1000) <= 100){
            tronco_idx = rand()%3;
            traslazione = rand()%3;

            if(traslazione < 2){

                for (size_t j = 0; j < N_MAX_ENEMY; j++)
                {
                    if(enemy[j].x == -1){
                        enemy[j].x = tronchi[tronco_idx].x +(L_FROGGER*traslazione);
                        enemy[j].y = tronchi[tronco_idx].y;

                        for (size_t k = 0; k < N_MAX_ENEMY; k++)
                        {
                            if(enemy[k].x != -1 && k!=j){
                                if(player.y == enemy[j].y || enemy[k].y == enemy[j].y){
                                    enemy[j].x = -1;
                                    k = N_MAX_ENEMY;
                                }
                            }
                        }
                        j = N_MAX_ENEMY;
                        
                    }
                }
                
            }
            else{
                for (size_t j = 0; j < N_MAX_ENEMY; j++)
                {
                    if(enemy[j].x == -1 && tronchi[tronco_idx].id_sprite == ID_TRUNK_X3){
                        enemy[j].x = tronchi[tronco_idx].x +(L_FROGGER*traslazione);
                        enemy[j].y = tronchi[tronco_idx].y;

                        for (size_t k = 0; k < N_MAX_ENEMY; k++)
                        {
                            if(enemy[k].x != -1 && k!=j){
                                if(player.y == enemy[j].y || enemy[k].y == enemy[j].y){
                                    enemy[j].x = -1;
                                    k = N_MAX_ENEMY;
                                }
                            }
                        }
                        j = N_MAX_ENEMY; 
                    }
                }
            }
        }
}

void inizializza_enemy()
{
    for (size_t i = 0; i < N_MAX_ENEMY; i++)
    {
        enemy[i].id_sprite=ID_ENEMY;
        enemy[i].x = -1;
    }
    
}

void* gestione_enemy(void* enemy)
{
    oggetto_rana* Enemy = (oggetto_rana *) enemy;
    int time_sparo_oggetto;
    
    srand(pthread_self());
    time_sparo_oggetto = TIME_SPARO_MIN + rand()%RANGE_TIME_SPARO;

    while(running)
    {
        if(Enemy->x != -1)
        {
            pthread_mutex_lock (&semCurses);
            crea_proiettile_enemy(Enemy);
            pthread_mutex_unlock (&semCurses);
        }
        usleep(time_sparo_oggetto);
    }
}

void crea_proiettile_enemy(oggetto_rana *player)
{
    for (int i = 0; i < N_MAX_P; i++)
    {
        if(proiettili_nemici[i].x  == -1){
            proiettili_nemici[i].x = player->x + 4;
            proiettili_nemici[i].y = player->y + H_FROGGER;

            i = N_MAX_P;
        } 
    }   
}

void stampa_enemy()
{   
    init_pair(ENEMY, COLOR_YELLOW, COLOR_BLUE);
    for (size_t i = 0; i < N_MAX_ENEMY; i++)
    {
        if(enemy[i].x != -1)
        {
            wattron(win_mappa, COLOR_PAIR(ENEMY));
            print_sprite(enemy[i].x, enemy[i].y, FROGGER);
        }
    }   
}