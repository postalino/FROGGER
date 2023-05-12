#include "NPC.h"

#define SPOSTA_SU -4
#define SPOSTA_GIU +4
#define SPOSTA_DESTRA +9
#define SPOSTA_SINISTRA -9
#define SPACE 32

#define TIME_SPARO 2500000

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
            // segnale di sparo
            break;
        }
        pthread_mutex_unlock (&semCurses);
    }
}