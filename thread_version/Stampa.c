#include "Stampa.h"

int delay_lettura = 0; //serve per gestire il momento della lettura
int max_time = 60; //tempo max in secondi per raggiungere una tana 

void play_frogger()
{
    vite = 3;   //vite iniziali
    
    while (true)
    {
        //operazioni di stampa oggetti aggiornati + mappa
        pthread_mutex_lock (&semCurses);

        /*  MAPPA   */
        wclear(win_mappa);
        mappa_frogger();

        /*   TRONCHI     */
        stampa_tronchi();

        /*  PLAYER   */
        init_pair(RANA,COLOR_WHITE,calcola_background(player.x,player.y));
        wattron(win_mappa,COLOR_PAIR(RANA));
        print_sprite(player.x, player.y, FROGGER);

        wrefresh(win_mappa);
        
        pthread_mutex_unlock (&semCurses);

        usleep(TIME_MAIN);
        
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

int abilita_movimento_confini_mappa(int direzione)
{
    if(direzione == -L_FROGGER && (player.x - L_FROGGER) < 30)
        return 0;
    if(direzione == L_FROGGER && (player.x + L_FROGGER) >= MAXX - 30)
        return 0;
    if(direzione == H_FROGGER && (player.y + L_FROGGER) > MAXY)
        return 0;
    if (player.y == MAX_PRATO && direzione == -H_FROGGER)
        return 0;
    else
        return 1;
}

int calcola_background(int x, int y)
{
    if (y >= MIN_FIUME && y <= MAX_FIUME)
        return COLOR_BLUE;
    if (y > MAX_FIUME && y < MIN_STRADA)
        return COLOR_MAGENTA;
    if (y >= MIN_STRADA && y <= MAX_STRADA)
        return COLOR_BLACK;
    if (y > MAX_STRADA)
        return COLOR_MAGENTA;
    if ((y >= MAX_PRATO && y < MAX_PRATO +H_FROGGER) && !((x / L_FROGGER) % 2))
        return COLOR_BLUE;
    if ((y >= MAX_PRATO && y < MAX_PRATO +H_FROGGER) && ((x / L_FROGGER) % 2))
        return COLOR_GREEN;
    if (y < H_FROGGER)
        return COLOR_GREEN;
}

void mappa_frogger()
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

    //print_barra_tempo(fd_time); //stampa barra del tempo di gioco più numero vite player
    //print_tane_occupate(tane_gioco); 
        
}

void print_sprite(int x, int y, const char *sprite[])
{
    int i = 0;
    while (sprite[i] != NULL) {  // scorri l'array finché non raggiungi la fine
        mvwprintw(win_mappa, y + i, x, sprite[i]);
        i++;
    }
}

void print_sprite_menu(WINDOW *name_win, int x, int y, const char *sprite[])
{
    int i = 0;
    while (sprite[i] != NULL) {  // scorri l'array finché non raggiungi la fine
        mvwprintw(name_win, y + i, x, sprite[i]);
        i++;
    }
}