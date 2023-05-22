#include "Frogger.h"

#define INVIO 10
#define SCELTE_START 4
#define SCELTE_END 2
#define SCELTE_DIFFICOLTA 3

pthread_t tRana; 
pthread_t tTronchi[N_CORSIE_FIUME];
pthread_t tTempo;
pthread_t tVeicoli[N_VEICOLI];
pthread_t tProiettili_A[N_MAX_P];
pthread_t tProiettili_E[N_MAX_P];
pthread_t tEnemy[N_MAX_ENEMY];

void cleanup();
int menu_fine_partita(int risultato_partita);
int menu_iniziale();
void menu_difficolta();
void lettura_scores();

int main()
{   
    int risultato_partita = 1;
    int play_again, new_play;
    bool restart = false;

    //default difficoltà (semplice)
    velocita_oggetti = TIME_DEFAULT;
    frequenza_enemy = FREQUENZA_DEFAULT;

    do
    {
        //crea la finestra e attiva/disattiva i comandi richiesti
        initscr();
        noecho();
        cbreak();
        start_color();
        curs_set(0);

        new_play = menu_iniziale();

        if(new_play)
        {
            do
            {
                if(restart){
                    initscr();
                    noecho();
                    cbreak();
                    start_color();
                    curs_set(0);
                    restart = false;
                }

                //crea la finestra e attiva/disattiva i comandi richiesti
                initscr();
                noecho();
                cbreak();
                start_color();
                curs_set(0);

                win_mappa = crea_finestra();
                running = true;		// Gioco non ancora finito

                pthread_mutex_init (&semCurses, NULL); // Mutex di ncurses

                /*      VEICOLI     */
                inizializza_veicoli();
                for (size_t i = 0; i < N_VEICOLI; i++)
                {
                    CREATE_THREAD(tVeicoli[i],gestione_veicolo,(void*)&veicolo[i]);
                }
                
                
                /*    PLAYER    */
                CREATE_THREAD(tRana, wasd_rana, NULL);

                /*    TRONCHI    */
                inizializza_tronchi();
                for (size_t i = 0; i < N_CORSIE_FIUME; i++)
                {
                    CREATE_THREAD(tTronchi[i], gestione_tronco,(void*)&tronchi[i]);
                }

                /*  TEMPO DI GIOCO*/
                CREATE_THREAD(tRana, tempoDiGioco, NULL);

                /*  PROIETTILI  */
                inizializza_proiettili();
                for (size_t i = 0; i < N_MAX_P; i++)
                {
                    CREATE_THREAD(tProiettili_A[i],gestione_proiettili,(void*)&proiettili_alleati[i]);
                    CREATE_THREAD(tProiettili_E[i],gestione_proiettili,(void*)&proiettili_nemici[i]);
                }
                /* ENEMY */
                inizializza_enemy();
                for (size_t i = 0; i < N_MAX_ENEMY; i++)
                {
                    CREATE_THREAD(tEnemy[i],gestione_enemy,(void*)&enemy[i]);
                }

                //inizio gioco
                risultato_partita = play_frogger();
                // quando il gioco è finito, chiamare la funzione di pulizia
                cleanup();
                endwin();

                initscr();
                noecho();
                cbreak();
                start_color();
                curs_set(0);

                play_again = menu_fine_partita(risultato_partita);
                if(play_again)
                    restart = true;
                
                endwin();

            }while(play_again);
        }
    }while(new_play);

    endwin();
    return 0;		// Termine del programma
}

void cleanup()
{
    /* Attendo la terminazione del thread */
    pthread_join (tRana, NULL);
    pthread_join (tTempo, NULL);
    for (size_t i = 0; i < N_VEICOLI; i++)
    {
        pthread_join (tVeicoli[i], NULL);
    }
    for (size_t i = 0; i < N_CORSIE_FIUME; i++)
    {
        pthread_join (tTronchi[i], NULL);
        pthread_join (tEnemy[i], NULL);
    }
    for (size_t i = 0; i < N_MAX_P; i++)
    {
        pthread_join (tProiettili_A[i], NULL);
        pthread_join (tProiettili_E[i], NULL);
    }
    

    /* Elimino il mutex ed il semaforo */
    pthread_mutex_destroy (&semCurses);
    wclear(win_mappa);			
    delwin(win_mappa);
}

int menu_iniziale()
{
    int choice;
    int highlight = 0;
    
    WINDOW *menu_partita;
    menu_partita = crea_finestra();
    CHECK_WINDOW(menu_partita); //verifica se a finestra e' stata creata correttamente
    keypad(menu_partita, true);

    char *scelte[SCELTE_START] = {"NUOVA PARTITA", "DIFFICOLTA'", "BEST SCORES","EXIT"};
    while (true)
    {
        wclear(menu_partita);
        print_sprite_menu(menu_partita,MAXX/10, MAXY/10, START);
        
        print_sprite_menu(menu_partita, MAXX/2 -35,MAXY/2 -2, R_LEFT_LOSE);
        print_sprite_menu(menu_partita, MAXX/2 + 15,MAXY/2 -1, R_RIGHT_LOSE);

        for (size_t i = 0; i < SCELTE_START; i++)
        {
            if (i == highlight)
                wattron(menu_partita, A_REVERSE);
            if(i == 0)
                mvwprintw(menu_partita, MAXY/2+i, MAXX/2-9, scelte[i]);
            else if(i== 1 || i== 2)
                mvwprintw(menu_partita, MAXY/2+i, MAXX/2-9+1, scelte[i]);
            else
                mvwprintw(menu_partita, MAXY/2+i, (MAXX/2-9)+4, scelte[i]);
            wattroff(menu_partita, A_REVERSE);
        }

        wrefresh(menu_partita);

        timeout(0);
        choice = wgetch(menu_partita);

        switch (choice)
        {
        case KEY_UP:
            highlight--;
            if(highlight < 0)
                highlight = SCELTE_START -1;
            break;
        case KEY_DOWN:
            highlight++;
            if(highlight > SCELTE_START -1)
                highlight = 0;
            break;
        case INVIO:
            switch (highlight)
            {
            case 0:
                delwin(menu_partita);
                return 1;
                break;
            case 1:
                menu_difficolta();
                break;
            case 2:
                wclear(menu_partita);
                lettura_scores();
                break;
            case 3:
                return 0;
                break;
            default:
                break;
            }
        default:
            break;
        }
    }
}

void lettura_scores()
{
    FILE *file;
    WINDOW *punteggio;
    int MAX_DIMENSIONE_RIGA = 25;
    char riga[MAX_DIMENSIONE_RIGA];
    int i= 0, choice = -1;
    bool continua = true;
    int highlight = 0;

    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x); // Ottieni la dimensione dello schermo stdscr

    int win_y = (max_y - MAXY) / 2; // Posizione Y della finestra centrata
    int win_x = (max_x - MAXX) / 2; // Posizione X della finestra centrata

    punteggio = newwin(MAXY, MAXX+10, win_y, win_x); // Crea una finestra mappa centrata
    CHECK_WINDOW(punteggio); //verifica se a finestra e' stata creata correttamente
    keypad(punteggio, true);
    // Apriamo il file in modalità di lettura
    file = fopen("scores.txt", "r");

    // Verifichiamo se il file è stato aperto correttamente
    if (file == NULL) {
        exit(0);
    }

    nodelay(punteggio, TRUE); // Imposta la modalità non bloccante per l'input da tastiera

    do{
        print_sprite_menu(punteggio,MAXX/10-15, MAXY/10-3, BEST_PLAYER);
        print_sprite_menu(punteggio, MAXX/2 -40,MAXY/2 -9, SCORE_WIN);
        print_sprite_menu(punteggio, MAXX/2 + 9 ,MAXY/2 -9, SCORE_WIN);

        // Leggiamo e stampiamo il contenuto del file riga per riga
        while (fgets(riga, MAX_DIMENSIONE_RIGA, file) != NULL) {

            mvwprintw(punteggio, MAXY/2+i, MAXX/2-9,"%s", riga);
            i+= 2;
        }

        mvwprintw(punteggio, MAXY/2 + 6, MAXX/2-21,"***************************************");
        mvwprintw(punteggio, MAXY/2 + 7, MAXX/2-21,"* ");
        if (highlight){
                wattron(punteggio, A_REVERSE);
            }
        mvwprintw(punteggio, MAXY/2 + 7, MAXX/2-19,"PREMI UN TASTO QUALSIASI PER USCIRE");
        wattroff(punteggio, A_REVERSE);
        mvwprintw(punteggio, MAXY/2 + 7, MAXX/2+16," *");
        mvwprintw(punteggio, MAXY/2 + 8, MAXX/2-21,"***************************************");
        wrefresh(punteggio);
        
        if(highlight)
            highlight = 0;
        else{
            highlight = 1;
        }

        timeout(1);
        choice = wgetch(punteggio);
        if(choice != -1)
            continua = false;
        
        wrefresh(punteggio);
        if(choice == -1)
            usleep(TIME_MAIN*3);
    }while (continua);
    
    wclear(punteggio);
    wrefresh(punteggio);
    
    //cancella la finestra
    delwin(punteggio);
    // Chiudiamo il file
    fclose(file);
}

void menu_difficolta()
{
    int choice;
    int highlight = 0;
    bool continua = true;
    // creazione finestra principale centrata
    WINDOW * selezione_difficolta;
    selezione_difficolta = crea_finestra();
    CHECK_WINDOW(selezione_difficolta); //verifica se a finestra e' stata creata correttamente
    keypad(selezione_difficolta, true);

    char *scelte[SCELTE_DIFFICOLTA] = {"FACILE", "MEDIA", "DIFFICILE"};

    do
    {   print_sprite_menu(selezione_difficolta,MAXX/10, MAXY/10, START);
        print_sprite_menu(selezione_difficolta, MAXX/2 -35,MAXY/2 -2, R_LEFT_LOSE);
        print_sprite_menu(selezione_difficolta, MAXX/2 + 15,MAXY/2 -1, R_RIGHT_LOSE);

        for (size_t i = 0; i < SCELTE_DIFFICOLTA; i++)
        {
            if (i == highlight)
                wattron(selezione_difficolta, A_REVERSE);
            mvwprintw(selezione_difficolta, MAXY/2+i, MAXX/2-9, scelte[i]);
            wattroff(selezione_difficolta, A_REVERSE);
        }

        wrefresh(selezione_difficolta);

        timeout(100);
        choice = wgetch(selezione_difficolta);

        switch (choice)
        {
        case KEY_UP:
            highlight--;
            if(highlight < 0)
                highlight = 1;
            break;
        case KEY_DOWN:
            highlight++;
            if(highlight > SCELTE_DIFFICOLTA -1 )
                highlight = 0;
            break;
        case INVIO:
            delwin(selezione_difficolta);
            switch (highlight)
            {
            case 0:
                velocita_oggetti = TIME_MAIN * 4;
                frequenza_enemy = 100;
                continua = false;
                break;
            case 1:
                velocita_oggetti = TIME_MAIN * 3;
                frequenza_enemy = 200;
                continua = false;
                break;
            case 2:
                velocita_oggetti = TIME_MAIN * 2;
                frequenza_enemy = 300;
                continua = false;
                break;
            default:
                break;
            }
        default:
            break;
        }
    }while(continua);
}

int menu_fine_partita(int risultato_partita)
{
    int choice;
    int highlight = 0;
    // creazione finestra principale centrata
    WINDOW * menu_fine_partita;
    menu_fine_partita = crea_finestra();
    CHECK_WINDOW(menu_fine_partita); //verifica se a finestra e' stata creata correttamente
    keypad(menu_fine_partita,true);

    if(risultato_partita) // SE VINCI REGISTRA IL TUO NOME
    {
        Giocatore giocatori[4];
        print_sprite_menu(menu_fine_partita,MAXX/10, MAXY/10, WIN);
        print_sprite_menu(menu_fine_partita, MAXX/2 -35,MAXY/2 -2, R_LEFT_LOSE);
        print_sprite_menu(menu_fine_partita, MAXX/2 + 15,MAXY/2 -1, R_RIGHT_LOSE);

        mvwprintw(menu_fine_partita, MAXY/2, MAXX/2-14,"Inserisci il tuo nickname:");
        mvwprintw(menu_fine_partita, MAXY/2+1, MAXX/2-14-1," ");
        wrefresh(menu_fine_partita);
        
        int i= 0,ch;
        curs_set(1);
        cbreak();
        do {
            
            ch = wgetch(menu_fine_partita);
            if(ch != INVIO && ((ch >= 'A' || ch <= 'Z') || (ch >= 'a' || ch <= 'a') || (ch >= '0' || ch <= '9' ) || ch == ' ')){
                mvwprintw(menu_fine_partita, MAXY/2+1, MAXX/2-14 +i,"%c", ch);
                giocatori[0].nome[i] = ch;
                wrefresh(menu_fine_partita);   
                i++;
            }
        } while (ch != INVIO && i < MAX_NOME-1); // Continua finché non viene premuto invio o il nome raggiunge la dimensione massima
        giocatori[0].nome[i] = '\0';
        wclear(menu_fine_partita);
        curs_set(0);

        giocatori[0].punteggio = punti;

        wclear(menu_fine_partita);

        FILE* file = fopen("scores.txt","r");
        if(file == NULL)
        {
            exit(1);
        }

        for (size_t i = 1; i < 4; i++)
        {
            fscanf(file,"%s %d\n",giocatori[i].nome,&giocatori[i].punteggio);
        }

        ordina_giocatori(giocatori,4);
        scrivi_giocatori_file(giocatori,3,"scores.txt");
    }

    char *scelte[SCELTE_END] = {"MENU' PRINCIPALE", "  GIOCA ANCORA  "};

    while (true)
    {
        if(risultato_partita)
            print_sprite_menu(menu_fine_partita,MAXX/10, MAXY/10, WIN);
        else
            print_sprite_menu(menu_fine_partita,MAXX/10, MAXY/10, LOSE);
        
        print_sprite_menu(menu_fine_partita, MAXX/2 -35,MAXY/2 -2, R_LEFT_LOSE);
        print_sprite_menu(menu_fine_partita, MAXX/2 + 15,MAXY/2 -1, R_RIGHT_LOSE);

        for (size_t i = 0; i < SCELTE_END; i++)
        {
            if (i == highlight)
                wattron(menu_fine_partita, A_REVERSE);
            mvwprintw(menu_fine_partita, MAXY/2+i, MAXX/2-9, scelte[i]);
            wattroff(menu_fine_partita, A_REVERSE);
        }

        wrefresh(menu_fine_partita);

        timeout(1);
        choice = wgetch(menu_fine_partita);

        switch (choice)
        {
        case KEY_UP:
            highlight--;
            if(highlight < 0)
                highlight = 1;
            break;
        case KEY_DOWN:
            highlight++;
            if(highlight > SCELTE_END -1 )
                highlight = 0;
            break;
        case INVIO:
            delwin(menu_fine_partita);
            switch (highlight)
            {
            case 0:
                return 0;
                break;
            case 1:
                return 1;
                break;
            default:
                break;
            }
        default:
            break;
        }
    }
}

void ordina_giocatori(Giocatore* giocatori, int numGiocatori) {
    int i, j;
    for (i = 0; i < numGiocatori - 1; i++) {
        for (j = 0; j < numGiocatori - i - 1; j++) {
            if (giocatori[j].punteggio < giocatori[j+1].punteggio) {
                Giocatore temp = giocatori[j];
                giocatori[j] = giocatori[j+1];
                giocatori[j+1] = temp;
            }
        }
    }
}

void scrivi_giocatori_file(Giocatore* giocatori, int numGiocatori, const char* nomeFile) {
    FILE* file = fopen(nomeFile, "w");
    if (file == NULL) {
        printf("Errore nell'apertura del file.\n");
        exit(1);
    }

    for (int i = 0; i < numGiocatori; i++) {
        fprintf(file, "%s %d\n", giocatori[i].nome, giocatori[i].punteggio);
    }

    fclose(file);
}