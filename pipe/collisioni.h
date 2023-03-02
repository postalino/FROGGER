
#define MAX_PROIETTILI 15

int collisioni_rana_veicoli(oggetto_rana,oggetto_veicolo [N_VEICOLI]);

int genera_processo_proiettile(int fd[2],oggetto_rana npc_rana);

void muovi_proiettile(int fd,oggetto_rana npc_rana);

void aggiorna_proiettile(int fd,oggetto_proiettile *proiettile);

int collisioni_proiettile();

void stampa_proiettili(oggetto_proiettile[MAX_PROIETTILI]);
