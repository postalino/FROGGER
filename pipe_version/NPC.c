#include "NPC.h"

#define SPOSTA_SU -4
#define SPOSTA_GIU +4
#define SPOSTA_DESTRA +9
#define SPOSTA_SINISTRA -9
#define SPACE 32

#define TIME_SPARO_MIN 1000000
#define RANGE_TIME_SPARO 2500000

void wasd_rana(int fd_rana, int fd_sparo)
{
    int direzione;
    int incremento;
    int sparato = 1;

    
    while (true)
    {
        timeout(1);
        direzione = getch();

        switch (direzione)
        {
        case KEY_UP:
            incremento = SPOSTA_SU;
            write(fd_rana, &incremento, sizeof(incremento)); // scritture posizione successive
            break;
        case KEY_DOWN:
            incremento = SPOSTA_GIU;
            write(fd_rana, &incremento, sizeof(incremento)); // scritture posizione successive
            break;
        case KEY_LEFT:
            incremento = SPOSTA_SINISTRA;
            write(fd_rana, &incremento, sizeof(incremento)); // scritture posizione successive
            break;
        case KEY_RIGHT:
            incremento = SPOSTA_DESTRA;
            write(fd_rana, &incremento, sizeof(incremento)); // scritture posizione successive
            break;
        case SPACE:
            write(fd_sparo,&sparato, sizeof(sparato));  // segnale di sparo
            break;
        }

        if (win_mappa == NULL) {
        fprintf(stderr, "Errore della finestra.\n");
        exit(EXIT_FAILURE);
        }  

    }
}

void update_position_frogger(int spostamento, oggetto_rana* npc)
{
    if(spostamento != 0)    //se presente un movimento vedo di che tipo altriementi no
    {
        if(spostamento == +4 || spostamento == -4)
            npc->y+=spostamento;
        else if(spostamento == +9 || spostamento == -9)
            npc->x+=spostamento;
    }
}

void inizializza_enemy(int fd_enemy[N_MAX_ENEMY][2], pid_t processi_enemy[N_MAX_ENEMY])
{
    for (size_t i = 0; i < N_MAX_ENEMY; i++)
    {
        enemy[i].x = -1;
        enemy[i].id_sprite = ID_ENEMY;
        CHECK_PIPE(fd_enemy[i]);    //verifica se la pipe e' stata creata correttamente
        fcntl(fd_enemy[i][0],F_SETFL, O_NONBLOCK); //imposto la pipe come non bloccante
        CHECK_PID(processi_enemy[i]);
        if(processi_enemy[i] == 0){
            close(fd_enemy[i][0]);
            gestione_enemy(fd_enemy[i][1]);
        }
        close(fd_enemy[i][1]);
    }
}

void gestione_enemy(int fd_rana_enemy)
{
    int sparo = 1;

    int time_sparo_oggetto;
    
    srand(getpid());
    time_sparo_oggetto = TIME_SPARO_MIN + rand()%RANGE_TIME_SPARO;

    while (true)
    {     
        write(fd_rana_enemy,&sparo , sizeof(sparo)); //scrittura spostamento
        usleep(time_sparo_oggetto);
    } 
}

void lettura_enemy(int fd_enemy[N_MAX_ENEMY][2])
{
    int sparo_nemico = 0;

    for (size_t i = 0; i < N_MAX_ENEMY; i++)
    {
        read(fd_enemy[i][0], &sparo_nemico, sizeof(sparo_nemico));
        if(enemy[i].x != -1){
            if(sparo_nemico)
            {
                gestione_processi_proiettili(enemy[i]);
                sparo_nemico = 0;
            }
        }
    }
     

}

void genera_enemy(oggetto_rana player)
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