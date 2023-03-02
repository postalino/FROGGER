
void gestione_tronco(int fd_tronco, int corsia);
void generazione_processi_tronco(int fd_tronchi[N_CORSIE_FIUME][2], pid_t processo_tronco[N_CORSIE_FIUME]);
void lettura_pipe_tronchi(oggetto_tronco tronchi[N_CORSIE_FIUME], int fd_tronco[N_CORSIE_FIUME][2]);
void tronco_taxi(oggetto_rana *npc_rana, oggetto_tronco npc_tronco);
void funzione_tempo_gioco(WINDOW *wtime);
void stampa_tronchi (oggetto_tronco tronchi[N_CORSIE_FIUME]);