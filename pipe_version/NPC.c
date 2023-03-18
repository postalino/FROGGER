#include "NPC.h"

#define SPOSTA_SU -4
#define SPOSTA_GIU +4
#define SPOSTA_DESTRA +9
#define SPOSTA_SINISTRA -9

void wasd_rana(int fd_rana)
{
    int direzione;
    int incremento;

    
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