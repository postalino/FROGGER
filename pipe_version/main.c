#include "Frogger.h"

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

int main()
{
    //inizializzo il seme della rand con time
    srand(time(NULL));
    int risultato_partita;
    char play_again;
    int verso, dimensione_tronco;
    int traslazioni[N_CORSIE_FIUME];

    do{
        //crea la finestra e attiva/disattiva i comandi richiesti
        initscr();
        noecho();
        cbreak();
        keypad(stdscr, true);
        start_color();
        curs_set(0);

    
        // creazione finestra principale centrata
        win_mappa = crea_finestra();
        CHECK_WINDOW(win_mappa); //verifica se a finestra e' stata creata correttamente
    
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
        init_pair(PLAY_AGAIN, COLOR_WHITE, COLOR_BLACK);

        wattron(win_mappa, COLOR_PAIR(PLAY_AGAIN));
        if(risultato_partita){
            wclear(win_mappa);
            print_sprite(MAXX/10, MAXY/10, WIN);
        }
        else{
            wclear(win_mappa);
            print_sprite(MAXX/10, MAXY/10, LOSE);
            print_sprite(MAXX/2 -35,MAXY/2 -2, R_LEFT_LOSE);
            print_sprite(MAXX/2 + 15,MAXY/2 -1, R_RIGHT_LOSE);
        }

        //wattron(win_mappa, COLOR_PAIR(PLAY_AGAIN));
        mvwprintw(win_mappa, MAXY/2, MAXX/2-9, "MENU' PRINCIPALE");
        mvwprintw(win_mappa, MAXY/2+2, MAXX/2-7,"GIOCA ANCORA");
        wrefresh(win_mappa);

        play_again = getchar();
        
        // quando il gioco è finito, chiamare la funzione di pulizia
        cleanup();

        endwin();
        }while(play_again == 'y' || play_again == 'Y');

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

//funzione per chiudere tutte le pipe aperte
void close_all_pipes() {
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

