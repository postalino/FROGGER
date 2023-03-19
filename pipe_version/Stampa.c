#include "Stampa.h"

int max_time = 60, vite = 3;

void play_frogger(int fd_time,int fd_rana, int fd_tronchi[N_CORSIE_FIUME][2])
{
    oggetto_rana player = {X_START , Y_START, ID_FROGGER, false};
    int movimento_rana = 0;

    while (true)
    {
        //operazioni di aggiornamenti degli oggetti
        /*   RANA     */
        read(fd_rana, &movimento_rana, sizeof(movimento_rana));
        if(abilita_movimento_confini_mappa(player, movimento_rana))
        {
            update_position_frogger(movimento_rana,&player);
            init_pair(RANA,COLOR_WHITE,calcola_background(player.x,player.y));
        }
        movimento_rana = 0;
        /*   TRONCO    */
        lettura_pipe_tronchi(tronchi,fd_tronchi);


        //operazioni di stampa oggetti aggiornati + mappa
        mappa_frogger(fd_time);
        stampa_tronchi(tronchi);
        wattron(win_mappa,COLOR_PAIR(RANA));
        print_sprite(player.x, player.y, FROGGER);
        
        wrefresh(win_mappa);
        usleep(100000);
        wclear(win_mappa);
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
    init_pair(T_MAX_WORD, COLOR_GREEN, COLOR_BLACK);     // tempo max
    init_pair(T_MIDLE_WORD, COLOR_YELLOW, COLOR_BLACK);    // tempo medio
    init_pair(T_MIN_WORD, COLOR_RED, COLOR_BLACK);       // tempo min

    //colore barra
    init_pair(T_MAX_BAR, COLOR_GREEN, COLOR_GREEN);     // tempo max
    init_pair(T_MIDLE_BAR, COLOR_YELLOW, COLOR_YELLOW);   // tempo medio
    init_pair(T_MIN_BAR, COLOR_RED, COLOR_RED);         // tempo min
    
    //quando passa un secondo ricevo dal figlio il clock per il decremento
    read(fd_time, &clock_secondo, sizeof(clock_secondo));
    max_time-=clock_secondo;
    
    //stampa scritta TIME
        if(max_time >30)
            wattron(win_mappa,COLOR_PAIR(T_MAX_WORD));
        else if(max_time >10 && max_time <= 30)
            wattron(win_mappa,COLOR_PAIR(T_MIDLE_WORD));
        else
            wattron(win_mappa,COLOR_PAIR(T_MIN_WORD));
        mvwprintw(win_mappa,MAXY-1,30,"TIME:");
        mvwprintw(win_mappa,MAXY-1,MAXX - 37,"VITE: %d",vite);

    //stampa barra
        if(max_time >30)
            wattron(win_mappa,COLOR_PAIR(T_MAX_BAR));
        else if(max_time >10 && max_time <= 30)
            wattron(win_mappa,COLOR_PAIR(T_MIDLE_BAR));
        else
            wattron(win_mappa,COLOR_PAIR(T_MIN_BAR));

        for (int i = 0; i < max_time; i++)
        {
            mvwprintw(win_mappa,MAXY-1,36+i," ");
        }
}

void mappa_frogger(int fd_time)
{
    // colori scenari
    init_pair(PRATO, COLOR_GREEN, COLOR_GREEN);     // prato verde
    init_pair(FIUME, COLOR_BLUE, COLOR_BLUE);       // fiume
    init_pair(MARCIAPIEDE, COLOR_MAGENTA, COLOR_MAGENTA); // marciapiede viola
    init_pair(STRADA, COLOR_BLACK, COLOR_BLACK);     // strada

    for (unsigned i = 0; i < H_FROGGER; i++) // generazione prato
    {

        wattron(win_mappa, COLOR_PAIR(STRADA));
        wprintw(win_mappa, "                              ");
        for (size_t i = 0; i < MAXX - 60; i++)
        {
            wattron(win_mappa, COLOR_PAIR(PRATO));
            wprintw(win_mappa, " ");
        }
        wattron(win_mappa, COLOR_PAIR(STRADA));
        wprintw(win_mappa, "                              ");
    }

    for (unsigned i = 0; i < H_FROGGER; i++) // generazione tane
    {
        wattron(win_mappa, COLOR_PAIR(STRADA));
        wprintw(win_mappa, "                              ");
        for (size_t i = 0; i < 11; i++)
        {
            if (i % 2 == 0)
            {
                wattron(win_mappa, COLOR_PAIR(PRATO));
                wprintw(win_mappa, "         ");
            }
            else
            {
                wattron(win_mappa, COLOR_PAIR(FIUME));
                wprintw(win_mappa, "         ");
            }
        }
        wattron(win_mappa, COLOR_PAIR(STRADA));
        wprintw(win_mappa, "                              ");
    }

    for (unsigned i = 0; i < H_FROGGER * N_CORSIE_FIUME; i++) // generazione fiume
    {
        wattron(win_mappa, COLOR_PAIR(STRADA));
        wprintw(win_mappa, "                              ");
        wattron(win_mappa, COLOR_PAIR(FIUME));
        for (size_t i = 0; i < MAXX - 60; i++)
        {
            wprintw(win_mappa, " ");
        }
        wattron(win_mappa, COLOR_PAIR(STRADA));
        wprintw(win_mappa, "                              ");
    }

    for (unsigned i = 0; i < H_FROGGER; i++) // generazione marciapiede
    {
        wattron(win_mappa, COLOR_PAIR(STRADA));
        wprintw(win_mappa, "                              ");
        wattron(win_mappa, COLOR_PAIR(MARCIAPIEDE));
        for (size_t i = 0; i < MAXX - 60; i++)
        {
            wprintw(win_mappa, " ");
        }
        wattron(win_mappa, COLOR_PAIR(STRADA));
        wprintw(win_mappa, "                              ");
    }

    for (unsigned i = 0; i < H_FROGGER * N_CORSIE_STRADA; i++) // generazione strada
    {
        wattron(win_mappa, COLOR_PAIR(STRADA));
        wprintw(win_mappa, "                              ");
        for (size_t i = 0; i < MAXX - 60; i++)
        {
            wprintw(win_mappa, " ");
        }
        wprintw(win_mappa, "                              ");
    }

    for (unsigned i = 0; i < H_FROGGER; i++) // generazione marciapiede
    {
        wattron(win_mappa, COLOR_PAIR(STRADA));
        wprintw(win_mappa, "                              ");
        wattron(win_mappa, COLOR_PAIR(MARCIAPIEDE));
        for (size_t i = 0; i < MAXX - 60; i++)
        {
            wprintw(win_mappa, " ");
        }
        wattron(win_mappa, COLOR_PAIR(STRADA));
        wprintw(win_mappa, "                              ");
    }

    print_barra_tempo(fd_time); //stampa barra del tempo di gioco
}