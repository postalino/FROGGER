#include "Stampa.h"

int max_time = 60, vite = 3;

void play_frogger(int fd_time,int fd_rana)
{
    oggetto_rana player = {X_START , Y_START, ID_FROGGER, false};
    int movimento_rana = 0;

    while (true)
    {
        wclear(win_mappa);

        read(fd_rana, &movimento_rana, sizeof(movimento_rana));
        init_pair(20,COLOR_WHITE,calcola_background(player.x,player.y));
        
        mappa_frogger(fd_time);
    
        if(abilita_movimento_confini_mappa(player, movimento_rana))
            update_position_frogger(movimento_rana,&player);
        movimento_rana = 0;

        wattron(win_mappa,COLOR_PAIR(20));
        print_sprite(player.x, player.y, FROGGER);
        wattroff(win_mappa,COLOR_PAIR(20));
        
        usleep(100000);
        wrefresh(win_mappa);
    }
}

WINDOW *crea_finestra()
{
    WINDOW *win;

    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x); // Ottieni la dimensione dello schermo stdscr

    int win_y = (max_y - MAXY) / 2; // Posizione Y della finestra centrata
    int win_x = (max_x - MAXX) / 2; // Posizione X della finestra centrata

    win = newwin(MAXY, MAXX, win_y, win_x); // Crea una finestra mappa centrata

    return win;
}

int abilita_movimento_confini_mappa(oggetto_rana npc, int direzione)
{
    if(direzione == -L_FROGGER && (npc.x - L_FROGGER) < 30)
        return 0;
    if(direzione == L_FROGGER && (npc.x + L_FROGGER) >= MAXX - 30)
        return 0;
    if(direzione == H_FROGGER && (npc.y + L_FROGGER) > MAXY)
        return 0;
    if ((npc.y == MAX_PRATO && !((npc.x / L_FROGGER) % 2)) && direzione != H_FROGGER)
        return 0;
    if ((npc.y == MIN_FIUME && ((npc.x / L_FROGGER) % 2)) && direzione == -H_FROGGER)
        return 0;
    else
        return 1;
}

int calcola_background(int x, int y)
{
    if (y >= MAX_PRATO && y <= MAX_FIUME)
        return COLOR_BLUE;
    if (y > MAX_FIUME && y < MIN_STRADA)
        return COLOR_MAGENTA;
    if (y >= MIN_STRADA && y <= MAX_STRADA)
        return COLOR_BLACK;
    if (y > MAX_STRADA)
        return COLOR_MAGENTA;
}

void print_sprite(int x, int y, const char *sprite[])
{
    for (int i = 0; i < H_FROGGER; i++)
    {
        mvwprintw(win_mappa,y + i, x, sprite[i]);
    }
}

void print_barra_tempo(int fd_time)
{
    int clock_secondo=0;

    //colore scritta TIME
    init_pair(5, COLOR_GREEN, COLOR_BLACK);     // tempo max
    init_pair(6, COLOR_YELLOW, COLOR_BLACK);    // tempo medio
    init_pair(7, COLOR_RED, COLOR_BLACK);       // tempo min

    //colore barra
    init_pair(8, COLOR_GREEN, COLOR_GREEN);     // tempo max
    init_pair(9, COLOR_YELLOW, COLOR_YELLOW);   // tempo medio
    init_pair(10, COLOR_RED, COLOR_RED);         // tempo min
    
    //se passa un secondo ricevo dal figlio il clock per il decremento
    read(fd_time, &clock_secondo, sizeof(clock_secondo));
    max_time-=clock_secondo;
    
    //stampa scritta TIME
        if(max_time >30)
            wattron(win_mappa,COLOR_PAIR(5));
        else if(max_time >10 && max_time <= 30)
            wattron(win_mappa,COLOR_PAIR(6));
        else
            wattron(win_mappa,COLOR_PAIR(7));
        mvwprintw(win_mappa,MAXY-1,30,"TIME:");

    //stampa barra
        if(max_time >30)
            wattron(win_mappa,COLOR_PAIR(8));
        else if(max_time >10 && max_time <= 30)
            wattron(win_mappa,COLOR_PAIR(9));
        else
            wattron(win_mappa,COLOR_PAIR(10));

        for (int i = 0; i < max_time; i++)
        {
            mvwprintw(win_mappa,MAXY-1,36+i," ");
        }

    //disabilito colore scritta TIME
        if(max_time >30)
            wattroff(win_mappa,COLOR_PAIR(5));
        else if(max_time >10 && max_time <= 30)
            wattroff(win_mappa,COLOR_PAIR(6));
        else
            wattroff(win_mappa,COLOR_PAIR(7));

    //disabilito colore barra
        if(max_time >30)
            wattroff(win_mappa,COLOR_PAIR(8));
        else if(max_time >10 && max_time <= 30)
            wattroff(win_mappa,COLOR_PAIR(9));
        else
            wattroff(win_mappa,COLOR_PAIR(10));
}

void mappa_frogger(int fd_time)
{
    // colori scenari
    init_pair(1, COLOR_GREEN, COLOR_GREEN);     // prato verde
    init_pair(2, COLOR_BLUE, COLOR_BLUE);       // mare
    init_pair(3, COLOR_MAGENTA, COLOR_MAGENTA); // prato viola
    init_pair(4, COLOR_BLACK, COLOR_BLACK);     // strada

    for (unsigned i = 0; i < H_FROGGER; i++) // generazione prato
    {

        wattron(win_mappa, COLOR_PAIR(4));
        wprintw(win_mappa, "                              ");
        wattroff(win_mappa, COLOR_PAIR(4));
        for (size_t i = 0; i < MAXX - 60; i++)
        {
            wattron(win_mappa, COLOR_PAIR(1));
            wprintw(win_mappa, " ");
        }
        wattroff(win_mappa, COLOR_PAIR(1));
        wattron(win_mappa, COLOR_PAIR(4));
        wprintw(win_mappa, "                              ");
        wattroff(win_mappa, COLOR_PAIR(4));
    }

    for (unsigned i = 0; i < H_FROGGER; i++) // generazione tane
    {
        wattron(win_mappa, COLOR_PAIR(4));
        wprintw(win_mappa, "                              ");
        wattroff(win_mappa, COLOR_PAIR(4));
        for (size_t i = 0; i < 11; i++)
        {
            if (i % 2 == 0)
            {
                wattron(win_mappa, COLOR_PAIR(1));
                wprintw(win_mappa, "         ");
                wattroff(win_mappa, COLOR_PAIR(1));
            }
            else
            {
                wattron(win_mappa, COLOR_PAIR(2));
                wprintw(win_mappa, "         ");
                wattroff(win_mappa, COLOR_PAIR(2));
            }
        }
        wattron(win_mappa, COLOR_PAIR(4));
        wprintw(win_mappa, "                              ");
        wattroff(win_mappa, COLOR_PAIR(4));
    }

    for (unsigned i = 0; i < H_FROGGER * N_CORSIE_FIUME; i++) // generazione fiume
    {
        wattron(win_mappa, COLOR_PAIR(4));
        wprintw(win_mappa, "                              ");
        wattroff(win_mappa, COLOR_PAIR(4));
        wattron(win_mappa, COLOR_PAIR(2));
        for (size_t i = 0; i < MAXX - 60; i++)
        {
            wprintw(win_mappa, " ");
        }
        wattroff(win_mappa, COLOR_PAIR(2));
        wattron(win_mappa, COLOR_PAIR(4));
        wprintw(win_mappa, "                              ");
        wattroff(win_mappa, COLOR_PAIR(4));
    }

    for (unsigned i = 0; i < H_FROGGER; i++) // generazione marciapiede
    {
        wattron(win_mappa, COLOR_PAIR(4));
        wprintw(win_mappa, "                              ");
        wattroff(win_mappa, COLOR_PAIR(4));
        wattron(win_mappa, COLOR_PAIR(3));
        for (size_t i = 0; i < MAXX - 60; i++)
        {
            wprintw(win_mappa, " ");
        }
        wattroff(win_mappa, COLOR_PAIR(3));
        wattron(win_mappa, COLOR_PAIR(4));
        wprintw(win_mappa, "                              ");
        wattroff(win_mappa, COLOR_PAIR(4));
    }

    for (unsigned i = 0; i < H_FROGGER * N_CORSIE_STRADA; i++) // generazione strada
    {
        wattron(win_mappa, COLOR_PAIR(4));
        wprintw(win_mappa, "                              ");
        for (size_t i = 0; i < MAXX - 60; i++)
        {
            wprintw(win_mappa, " ");
        }
        wprintw(win_mappa, "                              ");
        wattroff(win_mappa, COLOR_PAIR(4));
    }

    for (unsigned i = 0; i < H_FROGGER; i++) // generazione marciapiede
    {
        wattron(win_mappa, COLOR_PAIR(4));
        wprintw(win_mappa, "                              ");
        wattroff(win_mappa, COLOR_PAIR(4));
        wattron(win_mappa, COLOR_PAIR(3));
        for (size_t i = 0; i < MAXX - 60; i++)
        {
            wprintw(win_mappa, " ");
        }
        wattroff(win_mappa, COLOR_PAIR(3));
        wattron(win_mappa, COLOR_PAIR(4));
        wprintw(win_mappa, "                              ");
        wattroff(win_mappa, COLOR_PAIR(4));
    }

    print_barra_tempo(fd_time); //stampa barra del tempo di gioco
    mvwprintw(win_mappa,MAXY-1,MAXX - 37,"VITE: %d",vite);
}