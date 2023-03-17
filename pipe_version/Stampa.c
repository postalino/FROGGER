#include "Stampa.h"

void play_frogger()
{
    WINDOW *win_mappa = crea_finestra(); 
    
    while (true)
    {
        wclear(win_mappa);
        box(win_mappa, ACS_VLINE, ACS_HLINE);
        mappa_frogger(win_mappa);
    
        usleep(200000);
        
        wrefresh(win_mappa);


    }
}

WINDOW* crea_finestra()
{
    WINDOW *win;

    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x); // Ottieni la dimensione dello schermo stdscr

    int win_y = (max_y - MAXY) / 2; // Posizione Y della finestra centrata
    int win_x = (max_x - MAXX) / 2; // Posizione X della finestra centrata

    win = newwin(MAXY, MAXX, win_y, win_x); // Crea una finestra mappa centrata

    return win;
}

int abilita_movimento_confini_tane(oggetto_rana npc, int direzione)
{
    if((npc.y <9 && !((npc.x/9)%2)) && direzione ==KEY_UP)
        return 0;
    if((npc.y == 4 && ((npc.x/9)%2)) && direzione != KEY_DOWN)
        return 0;
    else
        return 1;
}

int calcola_background(int x , int y)
{
    if(y >= MAX_PRATO && y <= MAX_FIUME)
        return COLOR_BLUE;
    if(y > MAX_FIUME && y < MIN_STRADA)
        return COLOR_MAGENTA;
    if(y >= MIN_STRADA && y <= MAX_STRADA)
        return COLOR_BLACK;
    if(y>MAX_STRADA)
        return COLOR_MAGENTA;
}

void print_sprite(int x,int y,const char *sprite[], int size)
{
    for (int i = 0; i < size; i++)
    {
        mvprintw(y+i, x, sprite[i]);
    }
}

void mappa_frogger(WINDOW *win_mappa)
{

    // colori scenari
    init_pair(1, COLOR_GREEN, COLOR_GREEN);     // prato verde
    init_pair(2, COLOR_BLUE, COLOR_BLUE);       // mare
    init_pair(3, COLOR_MAGENTA, COLOR_MAGENTA); // prato viola
    init_pair(4, COLOR_BLACK, COLOR_BLACK);     // strada


    for (unsigned i = 0; i < H_FROGGER; i++) // generazione prato
    {
        
        wattron(win_mappa,COLOR_PAIR(4));
        wprintw(win_mappa,"                              ");
        for (size_t i = 0; i < MAXX - 60; i++)
        {
            wattron(win_mappa,COLOR_PAIR(1));
            wprintw(win_mappa," ");
        }
        wattron(win_mappa,COLOR_PAIR(4));
        wprintw(win_mappa,"                              ");
    }

    for (unsigned i = 0; i < H_FROGGER; i++) // generazione tane
    {
        wattron(win_mappa,COLOR_PAIR(4));
        wprintw(win_mappa,"                              ");
        for (size_t i = 0; i < 11; i++)
        {
            if(i%2 == 0){
                wattron(win_mappa, COLOR_PAIR(1));
                wprintw(win_mappa,"         ");
            }
            else{
                wattron(win_mappa,COLOR_PAIR(2));
                wprintw(win_mappa,"         ");
            }
            
        }
        wattron(win_mappa,COLOR_PAIR(4));
        wprintw(win_mappa,"                              ");
    }

    for (unsigned i = 0; i < H_FROGGER * N_CORSIE_FIUME; i++) // generazione fiume
    {
        wattron(win_mappa,COLOR_PAIR(4));
        wprintw(win_mappa,"                              ");
        wattron(win_mappa,COLOR_PAIR(2));
        for (size_t i = 0; i < MAXX - 60; i++)
        {
            wprintw(win_mappa," ");
        }
        wattron(win_mappa,COLOR_PAIR(4));
        wprintw(win_mappa,"                              ");
    }

    for (unsigned i = 0; i < H_FROGGER; i++) // generazione marciapiede
    {
        wattron(win_mappa,COLOR_PAIR(4));
        wprintw(win_mappa,"                              ");
        wattron(win_mappa,COLOR_PAIR(3));
        for (size_t i = 0; i < MAXX - 60; i++)
        {
            wprintw(win_mappa," ");
        }
        wattron(win_mappa,COLOR_PAIR(4));
        wprintw(win_mappa,"                              ");
    }

    for (unsigned i = 0; i < H_FROGGER * N_CORSIE_STRADA; i++) // generazione strada
    {
        wattron(win_mappa,COLOR_PAIR(4));
        wprintw(win_mappa,"                              ");
        wattron(win_mappa,COLOR_PAIR(4));
        for (size_t i = 0; i < MAXX - 60; i++)
        {
            wprintw(win_mappa," ");
        }
        wattron(win_mappa,COLOR_PAIR(4));
        wprintw(win_mappa,"                              ");
    }

    for (unsigned i = 0; i < H_FROGGER; i++) // generazione marciapiede
    {
        wattron(win_mappa,COLOR_PAIR(4));
        wprintw(win_mappa,"                              ");
        wattron(win_mappa,COLOR_PAIR(3));
        for (size_t i = 0; i < MAXX - 60; i++)
        {
            wprintw(win_mappa," ");
        }
        wattron(win_mappa,COLOR_PAIR(4));
        wprintw(win_mappa,"                              ");
    }
}