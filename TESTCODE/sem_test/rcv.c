#include "library.h"



int main(int argc, char const *argv[])
{
    struct sembuf op;
    sem_id = semget(0x22, 1 , IPC_CREAT|0600); 
    printf("[%d]CIAO QUESTO PROCESSO DOPO CHE FACCIO LE COSE MIE SBLOCCO I FIGLI DI MASTER\n",getpid());
    fflush(stdout);
    op.sem_flg = 0;
    op.sem_num = 0;
    op.sem_op = NUM_PROC+1; // Incrementa il semaforo
    sleep(1);
    semop(sem_id ,&op, 1); 
    return 0;
}
