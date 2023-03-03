#include "gestione_grafica.h"
#include "npc.h"
#define TIME 0


void wasd_rana(int fd_rana, int fd_trasporto_rana)
{
    oggetto_rana npc_rana = {L_FROGGER *5,MAXY -H_FROGGER,ID_FROGGER, false},npc_input ={0,0,0,false,0}; //centro la rana all'inizio della mappa di gioco
    int direzione = 0; 
    int timer_sparo =0;
    int old_y;   
    write(fd_rana, &npc_rana, sizeof(oggetto_rana));//scrittura posizione iniziale
    
    while(true)
    {
        timeout(0);
        direzione = getch();
        
        switch(direzione)
        {
            case KEY_UP:
                if(npc_rana.y > 0 && abilita_movimento_confini_tane(npc_rana, KEY_UP))
                { 
                   npc_rana.y -= H_FROGGER;
                   old_y = npc_rana.y;
                   write(fd_rana,&npc_rana , sizeof(oggetto_rana)); //scritture posizione successive
                   direzione = 0;
                }
                break;
            case KEY_DOWN:
                if(npc_rana.y + H_FROGGER < MAXY && abilita_movimento_confini_tane(npc_rana, KEY_DOWN))
                {
                    npc_rana.y += H_FROGGER;
                    old_y = npc_rana.y;
                    write(fd_rana,&npc_rana , sizeof(oggetto_rana)); //scritture posizione successive
                    direzione = 0;
                }    
                break;
            case KEY_LEFT:
                if(npc_rana.x > 0 && abilita_movimento_confini_tane(npc_rana, KEY_LEFT))
                {
                    npc_rana.x -= L_FROGGER;
                    write(fd_rana,&npc_rana , sizeof(oggetto_rana)); //scritture posizione successive
                    direzione = 0;
                }
                        
                break;
            case KEY_RIGHT:
                if(npc_rana.x + L_FROGGER < MAXX && abilita_movimento_confini_tane(npc_rana, KEY_RIGHT))
                {
                    npc_rana.x += L_FROGGER;
                    write(fd_rana,&npc_rana , sizeof(oggetto_rana)); //scritture posizione successive
                    direzione = 0;
                }
                break;
            case KEY_SPACE:
                if (timer_sparo == 0)
                {
                    npc_rana.sparo =1;
                    write(fd_rana,&npc_rana , sizeof(oggetto_rana)); //scritture posizione successive
                    npc_rana.sparo =0;
                    timer_sparo = 1000000;
                }
                break;
        }
        if (timer_sparo > 0)
        timer_sparo--;
        if(direzione != KEY_UP && direzione != KEY_DOWN && direzione != KEY_RIGHT && direzione != KEY_LEFT)
        {
            read(fd_trasporto_rana, &npc_input, sizeof(oggetto_rana));

            if(npc_input.sentinella_trasporto && npc_input.verso == 1 && (npc_rana.x < MAXX - L_FROGGER) && abilita_movimento_confini_tane(npc_rana, KEY_RIGHT) && old_y == npc_input.y)
            {
                npc_rana.x += L_FROGGER;
                npc_input.x = 0;
                npc_input.y = 0;
                npc_input.verso = 0;
                npc_input.sentinella_trasporto = false;

                usleep(TIME);

                write(fd_rana,&npc_rana , sizeof(oggetto_rana)); //scritture posizione successive
            }
            else if(npc_input.sentinella_trasporto && npc_input.verso == -1 && npc_rana.x > 0 && abilita_movimento_confini_tane(npc_rana, KEY_LEFT) && old_y == npc_input.y)
            {
                npc_rana.x -= L_FROGGER;
                npc_input.x = 0;
                npc_input.y = 0;
                npc_input.verso = 0;
                npc_input.sentinella_trasporto = false;
                
                usleep(TIME);
                write(fd_rana,&npc_rana , sizeof(oggetto_rana)); //scritture posizione successive
            }
        }
    }
}