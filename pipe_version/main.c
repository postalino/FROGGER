#include "Frogger.h"

#define INVIO 10
#define SCELTE_START 4
#define SCELTE_END 2
#define SCELTE_DIFFICOLTA 3

pid_t p_rana, p_tronco[N_CORSIE_FIUME],p_veicoli[N_VEICOLI], p_time, p_proiettile_alleati[N_MAX_P], p_enemy[N_MAX_ENEMY], p_proiettile_nemici[N_MAX_P];
int fd_veicolo [N_VEICOLI][2];
int fd_rana[2];
int fd_tronchi[N_CORSIE_FIUME][2];
int fd_time[2];
int fd_proiettile_alleati[N_MAX_P][2];
int fd_proiettile_nemici[N_MAX_P][2];
int fd_sparo[2];
int fd_enemy[N_MAX_ENEMY][2];
int fd_fine_manche[N_VEICOLI][2];

void close_all_pipes();
void close_all_child();
void cleanup();
int menu_fine_partita(int risultato_partita);
int menu_iniziale();
void menu_difficolta();
void lettura_scores();

int main()
{
    //inizializzo il seme della rand con time
    srand(time(NULL));
    int risultato_partita = 1;
    int play_again, new_play;
    int verso, dimensione_tronco;
    int traslazioni[N_CORSIE_FIUME];
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

    if(new_play){
        do{
            if(restart){
                initscr();
                noecho();
                cbreak();
                start_color();
                curs_set(0);
                restart = false;
            }

            // creazione finestra principale centrata
            win_mappa = crea_finestra();
            CHECK_WINDOW(win_mappa); //verifica se a finestra e' stata creata correttamente
            keypad(win_mappa, true);

            
            //calcolo randomicamente una traslazione iniziale per avere movimenti diversi dei tronchi
            for (int i = 0; i < N_CORSIE_FIUME; i++) {
                traslazioni[i] = (L_FROGGER*(rand()%4));  // genero l'incremento casuale [set casuale-->(0,9,18,27)]
                for (int j = 0; j < i; j++) {
                    if (traslazioni[j] == traslazioni[i]) {  // controllo se l'incremento è gia stato generato
                        i--;  // se sì, decrementa i per ripetere il ciclo e generarne un'altro
                        break;
                    }
                }
            }
            //inizializzo le strutture tronchi per avere la stessa coppia sia nel padre sia nel figlio
            for (int i = 0; i < N_CORSIE_FIUME; i++)
            {
                //determino randomicamente il verso del tronco ( 0 -. destra | 1 -. sinistra)
                verso = rand()%2;
                //determino randomicamente la dimensione del tronco (0 -. x2 | 1 -. x3)
                dimensione_tronco = rand()%2;
                //inizializza la struttura specifica
                inizializza_tronco(&tronchi[i], i, verso, dimensione_tronco, traslazioni[i]);
            }
                
            inizializza_veicoli();

            genera_processi_veicoli(fd_veicolo,p_veicoli,fd_fine_manche);

            inizializza_enemy(fd_enemy, p_enemy);

            //inizializzo le cordinate dei proiettili fuori mappa e creo le pipe di ogni proiettile
            inizializza_proiettili(fd_proiettile_alleati, p_proiettile_alleati, proiettili_alleati);
            inizializza_proiettili(fd_proiettile_nemici, p_proiettile_nemici, proiettili_nemici);

            generazione_processi_tronco(fd_tronchi,p_tronco,tronchi);

            CHECK_PIPE(fd_time);    //verifica se la pipe e' stata creata correttamente
            fcntl(fd_time[0],F_SETFL, O_NONBLOCK); //imposto la pipe come non bloccante
            CHECK_PID(p_time);  //creo processo figlio tempo di gioco
                
            switch (p_time)
            {
            case 0:
                /* processo figlio */
                close(fd_time[0]);  //chiusura file descriptor in lettura
                tempoDiGioco(fd_time[1]);
                break;
                
            default:
                /* processo padre */
                close(fd_time[1]); //chiusura file descriptor in scrittura
                break;
            }

            CHECK_PIPE(fd_sparo);    //verifica se la pipe e' stata creata correttamente
            fcntl(fd_sparo[0],F_SETFL, O_NONBLOCK); //imposto la pipe come non bloccante

            CHECK_PIPE(fd_rana);    //verifica se la pipe e' stata creata correttamente
            fcntl(fd_rana[0],F_SETFL, O_NONBLOCK); //imposto la pipe come non bloccante
            CHECK_PID(p_rana);  //creo processo figlio rana

            switch (p_rana)
            {
            case 0:
                /* processo figlio */
                close(fd_rana[0]);  //chiusura file descriptor in lettura
                close(fd_sparo[0]);  //chiusura file descriptor in lettura
                wasd_rana(fd_rana[1], fd_sparo[1]);
                break;
                
            default:
                /* processo padre */
                close(fd_rana[1]); //chiusura file descriptor in scrittura
                close(fd_sparo[1]); //chiusura file descriptor in scrittura
                break;
            }

            //inizio gioco
            risultato_partita = play_frogger(fd_time[0],fd_rana[0], fd_tronchi,fd_veicolo, fd_proiettile_alleati, fd_sparo[0], fd_enemy,fd_fine_manche);
            // quando il gioco è finito, chiamare la funzione di pulizia
            cleanup();
            usleep(20000);
            endwin();

            initscr();
            noecho();
            cbreak();
            start_color();
            curs_set(0);

            play_again = menu_fine_partita(risultato_partita);
            if(play_again)
                restart = true;
            
            usleep(20000);
            endwin();

            }while(play_again);

            usleep(10000);
            endwin();
        }

    
    // quando il gioco è finito, chiamare la funzione di pulizia
    cleanup();
    usleep(10000);
    endwin();
    }while(new_play);

    exit(0);
}

void cleanup()
{
    // chiudi tutti i processi figli in modo pulito
    close_all_child();

    // chiudi tutte le pipe aperte
    close_all_pipes();

    //chiudo la finestra
    delwin(win_mappa);
}

void close_all_pipes() {
    //funzione per chiudere tutte le pipe aperte
    for (int i = 0; i < N_VEICOLI; i++) {
        close(fd_veicolo[i][0]); //chiudi la lettura della pipe
        close(fd_veicolo[i][1]); //chiudi la scrittura della pipe
        close(fd_fine_manche[i][0]);
        close(fd_fine_manche[i][1]);
    }

    for (int i = 0; i < N_CORSIE_FIUME; i++) {
        close(fd_tronchi[i][0]); //chiudi la lettura della pipe
        close(fd_tronchi[i][1]); //chiudi la scrittura della pipe
        close(fd_enemy[i][0]);
        close(fd_enemy[i][1]);
    }

    for (int i = 0; i < N_MAX_P; i++) {
        close(fd_proiettile_alleati[i][0]); //chiudi la lettura della pipe
        close(fd_proiettile_alleati[i][1]); //chiudi la scrittura della pipe
        close(fd_proiettile_nemici[i][0]);
        close(fd_proiettile_nemici[i][1]);
    }

    close(fd_time[0]);
    close(fd_time[1]);

    close(fd_rana[0]);
    close(fd_rana[1]);

    close(fd_sparo[0]);
    close(fd_sparo[1]);
}

void close_all_child()
{
    for (int i = 0; i < N_VEICOLI; i++) {
        kill(p_veicoli[i], SIGTERM);
    }

    for (int i = 0; i < N_CORSIE_FIUME; i++) {
        kill(p_tronco[i], SIGTERM);
        kill(p_enemy[i], SIGTERM);
    }

    for (int i = 0; i < N_MAX_P; i++) {
        kill(p_proiettile_alleati[i], SIGTERM);
        kill(p_proiettile_nemici[i], SIGTERM);
    }

    kill(p_rana, SIGTERM);
    kill(p_time, SIGTERM);
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
                highlight = 1;
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

        mvwprintw(menu_fine_partita, MAXY/2-1, MAXX/2-14,"IL TUO SCORE E': %d",punti);
        mvwprintw(menu_fine_partita, MAXY/2, MAXX/2-14,"Inserisci il tuo nickname:");
        mvwprintw(menu_fine_partita, MAXY/2+1, MAXX/2-14-1," ");
        int i= -1,ch;
        curs_set(1);
        do {
            
            ch = wgetch(menu_fine_partita);
            if(ch != INVIO && ((ch >= 'A' || ch <= 'Z') || (ch >= 'a' || ch <= 'a') || (ch >= 48 || ch <= 57 ) || ch == ' ')){
                i++;
                giocatori[0].nome[i] = ch;
                mvwprintw(menu_fine_partita, MAXY/2+1, MAXX/2-14 +i,"%c", ch);
            }
            
            wrefresh(menu_fine_partita);
        } while (ch != INVIO && i < MAX_NOME-1); // Continua finché non viene premuto invio o il nome raggiunge la dimensione massima
        giocatori[0].nome[i+1] = '\0';
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
        
        
        mvwprintw(menu_fine_partita, MAXY/2-3, MAXX/2-10,"IL TUO SCORE E': %d",punti);
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

        timeout(100);
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