#include "Stampa.h"

int delay_lettura = 0; //serve per gestire il momento della lettura
int max_time = 60; //tempo max in secondi per raggiungere una tana 
int vite = 3;   //vite iniziali

void play_frogger(int fd_time,int fd_rana, int fd_tronchi[N_CORSIE_FIUME][2],int fd_veicoli[N_VEICOLI][2], int fd_proiettile_alleati[N_MAX_P][2], int fd_sparo, int fd_enemy[N_MAX_ENEMY][2])
{
    oggetto_rana player = {X_START , Y_START, ID_FROGGER};
    int movimento_rana = 0;
    int sparo = 0;

    //enemy[0].x = tronchi[0].x;
    //enemy[0].y = tronchi[0].y;

    inizializza_posizione_tane(tane_gioco); //assegno ad ogni tana le cordinate e il valore di non occupata

    while (true)
    {
        //operazioni di aggiornamenti degli oggetti
        /*   VEICOLI     */
        aggiorna_veicoli(fd_veicoli);
        /*   PROIETTILI     */
        lettura_proiettili(fd_proiettile_alleati, proiettili_alleati);
        lettura_proiettili(fd_proiettile_alleati, proiettili_nemici);
        
        /*   TANE     */
        if(tana_occupata(&player,tane_gioco)){
            //se la tana viene occupata si ripristina il tempo di gioco e si incrementa di 1 unità la vita
            vite++;
            max_time = 60;
        }
        fuori_area_tane(&player, &vite);

        /*   RANA     */
        read(fd_rana, &movimento_rana, sizeof(movimento_rana));
        if(abilita_movimento_confini_mappa(player, movimento_rana)){
            update_position_frogger(movimento_rana,&player);
            init_pair(RANA,COLOR_WHITE,calcola_background(player.x,player.y));
        }

        read(fd_sparo, &sparo, sizeof(sparo));
        
        movimento_rana = 0;

        /*   TRONCO   */
        delay_lettura++; //sincronizza la lettura del tronco una ogni tempo_tronco/tempo_padre
        if(delay_lettura == TIME/TIME_MAIN){
            lettura_pipe_tronchi(&player,tronchi,fd_tronchi, &vite, enemy); // vengono aggiornati i tronchi. Se la rana è sopra uno dei tronchi viene trasportata
            delay_lettura = 0;
        }

        /*   ENEMY     */
        genera_enemy();
        lettura_enemy(fd_enemy);

        //operazioni di stampa oggetti aggiornati + mappa
        mappa_frogger(fd_time);
        stampa_tronchi(tronchi);
        stampa_veicoli();
        wattron(win_mappa,COLOR_PAIR(RANA));
        print_sprite(player.x, player.y, FROGGER);
        stampa_enemy();

        if(sparo){
            gestione_processi_proiettili(player);
            sparo--;
        }        

        stampa_proiettili();
        collisioni_game(&player, &vite);
        for (size_t i = 0; i < max_enemy_reali; i++)
        {
            mvwprintw(win_mappa, 0 +i, 0, "%d) X: %d Y: %d", i+1,enemy[i].x, enemy[i].y ); 
        }
        

        wrefresh(win_mappa);
        usleep(TIME_MAIN);
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
    if (npc.y == MAX_PRATO && direzione == -H_FROGGER)
        return 0;
    if (check_tana(npc.x, npc.y) && direzione == -H_FROGGER)//controlla se la tana è già stata occupata
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
    
    //stampa scritta VITE
        if(vite >= 3)
            wattron(win_mappa,COLOR_PAIR(T_MAX_WORD));
        else if(vite == 2)
            wattron(win_mappa,COLOR_PAIR(T_MIDLE_WORD));
        else if(vite == 1)
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

    print_barra_tempo(fd_time); //stampa barra del tempo di gioco più numero vite player
    print_tane_occupate(tane_gioco); 
        
}

void print_sprite(int x, int y, const char *sprite[])
{
    for (int i = 0; i < H_FROGGER; i++)
    {
        mvwprintw(win_mappa,y + i, x, sprite[i]);
    }
}