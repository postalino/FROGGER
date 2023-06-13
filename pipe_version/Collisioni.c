#include "Collisioni.h"

void tempoDiGioco(int fd)
{
    int clock = 0;
    int tempo = 0;
    while (true)
    {
        sleep(1);
        clock++;
        write(fd,&clock , sizeof(clock)); //scritture tempo di gioco rimanente
        tempo++;
        clock--;
    }
}

int collisioni_rana_veicoli(oggetto_rana rana,oggetto_veicolo veicolo [N_VEICOLI])
{
    for (int i=0;i< N_VEICOLI;i++)
    {
        if (veicolo[i].id_sprite == 1 || veicolo[i].id_sprite == 4) //hitbox macchina
        {
            for(int j=0;j<14;j++)
            {
                if ((rana.x == veicolo[i].x + j && rana.y == veicolo[i].y) || (rana.x + 8 == veicolo[i].x + j && rana.y == veicolo[i].y))
                {
                    return 1;
                }
            }
        }else //hitbox camion/pullman
        {
            for(int j=0;j<28;j++)
            {
                if ((rana.x == veicolo[i].x + j && rana.y == veicolo[i].y) || (rana.x + 8 == veicolo[i].x + j && rana.y == veicolo[i].y))
                {
                    return 1;
                }
            }
        }
    }

    return 0;
}

void inizializza_proiettili(int fd_proiettile_alleati[N_MAX_P][2], pid_t processi[N_MAX_P], oggetto_proiettile proiettili[N_MAX_P])
{
    for (size_t i = 0; i < N_MAX_P; i++)
    {
        CHECK_PIPE(fd_proiettile_alleati[i]);    //verifica se la pipe e' stata creata correttamente
        proiettili[i].x = -1;
        CHECK_PID(processi[i]);

        if(processi[i] == 0){
            close(fd_proiettile_alleati[i][0]);
            gestione_proiettile(fd_proiettile_alleati[i][1]);
        }
        
        close(fd_proiettile_alleati[i][1]);    
    }
    
}

void gestione_proiettile(int fd_alleati)
{
    int spostamento = -1;
    while (true)
    {
        write(fd_alleati,&spostamento , sizeof(spostamento)); //scrittura spostamento
        usleep(TIME_MAIN);
    }
}

void gestione_processi_proiettili(oggetto_rana player)
{
    if(player.id_sprite == ID_FROGGER){
        for (int i = 0; i < N_MAX_P; i++)
        {
            if(proiettili_alleati[i].x  == -1){
                proiettili_alleati[i].x = player.x + 4;
                proiettili_alleati[i].y = player.y;
                proiettili_alleati[i].verso_proiettile = 1;
            
                i = N_MAX_P;
            } 
        }
    }
    else if(player.id_sprite == ID_ENEMY){
        for (int i = 0; i < N_MAX_P; i++)
        {
            if(proiettili_nemici[i].x  == -1){
                proiettili_nemici[i].x = player.x + 4;
                proiettili_nemici[i].y = player.y + H_FROGGER;
                proiettili_nemici[i].verso_proiettile = -1;
            
                i = N_MAX_P;
            } 
        }
    }
    
}

void lettura_proiettili(int fd_alleati[N_MAX_P][2], oggetto_proiettile proiettili[N_MAX_P])
{
    int spostamento = 0;

    for (size_t i = 0; i < N_MAX_P; i++)
    {
        if(proiettili[i].x != -1)
        {
            read(fd_alleati[i][0],&spostamento, sizeof(spostamento));
            proiettili[i].y += spostamento*proiettili[i].verso_proiettile;
            spostamento = 0;
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

void collisioni_proiettile_enemy()
{
    for (size_t i = 0; i < N_MAX_ENEMY; i++)
    {
        if(enemy[i].x != -1){
            for (size_t j = 0; j < N_MAX_P; j++)
            {
                if((proiettili_alleati[j].x == enemy[i].x +H_FROGGER) && (proiettili_alleati[j].y >= enemy[i].y && proiettili_alleati[j].y <= enemy[i].y +H_FROGGER ) )
                {
                    punti+= 5;
                    enemy[i].x = -1;
                    proiettili_alleati[j].x = -1;
                }
            }
        }
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

void collisioni_proiettili_macchine()
{
    for (size_t i = 0; i < N_VEICOLI; i++)
    {
        for (size_t l = 0; l < N_MAX_P; l++) // proiettili alleati
        {
            if (veicoli[i].id_sprite == 1 || veicoli[i].id_sprite == 4) //hitbox macchina
            {
                if ( (proiettili_alleati[l].x >= veicoli[i].x && proiettili_alleati[l].x <= veicoli[i].x + 13) && (proiettili_alleati[l].y == veicoli[i].y + (H_FROGGER -1)))
                {
                    proiettili_alleati[l].x = -1;
                }
                if( (proiettili_alleati[l].y >= veicoli[i].y && proiettili_alleati[l].y <= veicoli[i].y + (H_FROGGER -1)) && ((proiettili_alleati[l].x == veicoli[i].x)||(proiettili_alleati[l].x == veicoli[i].x+13)))
                {
                    proiettili_alleati[l].x = -1;
                }
            }
            else //hitbox camion/pullman
            {
                if ( (proiettili_alleati[l].x >= veicoli[i].x && proiettili_alleati[l].x <= veicoli[i].x + 27) && (proiettili_alleati[l].y == veicoli[i].y + (H_FROGGER -1)))
                {
                    proiettili_alleati[l].x = -1;
                }
                if( (proiettili_alleati[l].y >= veicoli[i].y && proiettili_alleati[l].y <= veicoli[i].y + (H_FROGGER -1)) && ((proiettili_alleati[l].x == veicoli[i].x)||(proiettili_alleati[l].x == veicoli[i].x+27)))
                {
                    proiettili_alleati[l].x = -1;
                }
            }
         } 
        for (size_t l = 0; l < N_MAX_P; l++) //proiettili nemici
        {
            if (veicoli[i].id_sprite == 1 || veicoli[i].id_sprite == 4) //hitbox macchina
            {
                for(int j=0;j<14;j++)
                {
                    if (proiettili_nemici[l].x == veicoli[i].x + j && proiettili_nemici[l].y == veicoli[i].y)
                    {
                        proiettili_nemici[l].x = -1;
                    }else
                    {
                        for (size_t m = 1; m < 4; m++)
                        {
                            if ((proiettili_nemici[l].x == veicoli[i].x && proiettili_nemici[l].y == veicoli[i].y + m)||(proiettili_nemici[l].x == veicoli[i].x + 13 && proiettili_nemici[l].y == veicoli[i].y + m))
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
                    if (proiettili_nemici[l].x == veicoli[i].x + j && proiettili_nemici[l].y == veicoli[i].y)
                    {
                        proiettili_nemici[l].x = -1;
                    }else
                    {
                        for (size_t m = 1; m < 4; m++)
                        {
                            if ((proiettili_nemici[l].x == veicoli[i].x && proiettili_nemici[l].y == veicoli[i].y + m)||(proiettili_nemici[l].x == veicoli[i].x + 27 && proiettili_nemici[l].y == veicoli[i].y + m))
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

void collisione_player_enemy(oggetto_rana *player, int* vite)
{
    for (size_t i = 0; i < N_MAX_ENEMY; i++)
    {
        if(enemy[i].x!= -1){
            if(enemy[i].y == player->y)
            {
                player->x = X_START;
                player->y = Y_START;
                (*vite)--;
            }
        }
    }
}

void collisione_player_proiettileN(oggetto_rana *player, int *vite)
{
    for (size_t i = 0; i < N_MAX_ENEMY; i++)
    {
        if(proiettili_nemici[i].x!= -1){
            if((proiettili_nemici[i].x == player->x +H_FROGGER) && (proiettili_nemici[i].y >= player->y && proiettili_nemici[i].y <= player->y +H_FROGGER ))
            {
                player->x = X_START;
                player->y = Y_START;
                proiettili_nemici[i].x = -1;
                (*vite)--;
            }
        }
    }
}

void collisione_fine_tempo(int* tempo, oggetto_rana *player, int* vite )
{
    if(*tempo <= 0){
        player->x = X_START;
        player->y = Y_START;
        (*tempo) = 60;
        (*vite)--;
    }
}
void collisioni_game(oggetto_rana *player, int* vite, int *tempo)
{
    collisione_fine_tempo(tempo,player,vite);
    collisioni_proiettili_macchine();
    collisioni_proiettili_bordi();
    collisioni_proiettile_enemy();
    collisione_player_proiettileN(player, vite);
    collisione_player_enemy(player, vite);
}
