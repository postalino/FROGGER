#include "Strada.h"

void genera_processi_veicoli(int fd_veicolo[N_VEICOLI][2],pid_t pid_veicoli[N_VEICOLI])
{

    srand(getpid());
    int prima_strada=rand()%2; //variabile per decidere se la prima strada andrà verso destra o verso sinistra

    for(int i =0;i<N_VEICOLI;i++)
    {
        CHECK_PIPE(fd_veicolo[i]);
        CHECK_PID(pid_veicoli[i]);
        if(!pid_veicoli[i])
        {
            while (true)
            {
                /* code */
            }
        }
    }
}