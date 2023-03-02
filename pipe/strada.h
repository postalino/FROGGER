
#define MAXX 99

#define STRADA_1 36
#define STRADA_2 40
#define STRADA_3 44

#define N_VEICOLI 6

#define CHECK_PIPE(x) {if(pipe(x)== -1) {perror("pipe call"); _exit(1);} }
#define CHECK_PID(x) {x= fork(); if(x < 0) {perror("fork call");_exit(2);}}


//prototipo funzione per generare i 6 diversi processi dei 6 veicoli
void genera_processi_veicoli(int[N_VEICOLI][2],pid_t[N_VEICOLI]);
//prototipo funzione che genera e scrive nella pipe le coordinate di un veicolo partendo da una certa x andando verso destra
void genera_coord_veicolo_right(int,int,int);
//prototipo funzione che genera e scrive nella pipe le coordinate di un veicolo partendo da una certa x andando verso sinistra
void genera_coord_veicolo_left(int,int,int);

void stampa_veicoli(oggetto_veicolo[]);

