#include "Stampa.h"

int play_frogger()
{
    pthread_mutex_lock (&semCurses);
    max_time = 60; //tempo max in secondi per raggiungere una tana
    vite = 3;   //vite iniziali
    int backup_vite = 3;
    inizializza_posizione_tane();
    pthread_mutex_unlock (&semCurses);

    do
    {
        //operazioni di stampa oggetti aggiornati + mappa
        pthread_mutex_lock (&semCurses);

        /*   TANE     */
        if(tana_occupata()){
            punti += max_time;
            max_time = 60;
        }
        fuori_area_tane();

        /*  MAPPA   */
        wclear(win_mappa);
        mappa_frogger();

        /*   TRONCHI     */
        stampa_tronchi();

        /*  PLAYER   */
        init_pair(RANA,COLOR_WHITE,calcola_background(player.x,player.y));
        wattron(win_mappa,COLOR_PAIR(RANA));
        print_sprite(player.x, player.y, FROGGER);

        /*  ENEMY  */
        genera_enemy();
        stampa_enemy();

        /*   VEICOLI  */
        stampa_veicoli();

        /* PROIETTILI */
        collisioni_proiettili_bordi();
        collisioni_proiettili_macchine();
        stampa_proiettili();

        /*  COLLISIONI  */
        collisioni_game();

        wrefresh(win_mappa);

        if (backup_vite != vite){
            for (size_t i = 0; i < N_VEICOLI; i++)
            {
                finemanche[i]=1;
            }
            
            backup_vite --;
        }

        game_over(); //verifica se il gioco è finito

        pthread_mutex_unlock (&semCurses);

        usleep(TIME_MAIN);
        
    }while (running);

    return vittoria();
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

    print_barra_tempo(); //stampa barra del tempo di gioco più numero vite player
    print_tane_occupate(); 
        
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

void print_barra_tempo()
{

    //colore scritta TIME
    init_pair(T_MAX_WORD, COLOR_GREEN, COLOR_BLACK);     // tempo max
    init_pair(T_MIDLE_WORD, COLOR_YELLOW, COLOR_BLACK);    // tempo medio
    init_pair(T_MIN_WORD, COLOR_RED, COLOR_BLACK);       // tempo min

    //colore barra
    init_pair(T_MAX_BAR, COLOR_GREEN, COLOR_GREEN);     // tempo max
    init_pair(T_MIDLE_BAR, COLOR_YELLOW, COLOR_YELLOW);   // tempo medio
    init_pair(T_MIN_BAR, COLOR_RED, COLOR_RED);         // tempo min
    
    //stampa scritta TIME
        if(max_time >30)
            wattron(win_mappa,COLOR_PAIR(T_MAX_WORD));
        else if(max_time >10 && max_time <= 30)
            wattron(win_mappa,COLOR_PAIR(T_MIDLE_WORD));
        else
            wattron(win_mappa,COLOR_PAIR(T_MIN_WORD));
        mvwprintw(win_mappa,MAXY-1,30,"TIME:");
    
    //stampa scritta VITE
        if(vite >= 3)
            wattron(win_mappa,COLOR_PAIR(T_MAX_WORD));
        else if(vite == 2)
            wattron(win_mappa,COLOR_PAIR(T_MIDLE_WORD));
        else if(vite <= 1)
            wattron(win_mappa,COLOR_PAIR(T_MIN_WORD));
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
        //stampa score
    wattron(win_mappa,COLOR_PAIR(T_MAX_WORD));
    mvwprintw(win_mappa,MAXY-1,MAXX-51,"SCORE : %d", punti);
}
