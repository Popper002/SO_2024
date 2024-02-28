#include "library.h"



int main(int argc, char const *argv[])
{
     struct sembuf op;
     sem_id = semget(0x22, 1 , IPC_CREAT|0600); 
    printf("[%d]CIAO QUESTO PROCESSO DOPO CHE FACCIO LE COSE MIE SBLOCCO I FIGLI DI MASTER\n",getpid());
     
        
    return 0;
}
