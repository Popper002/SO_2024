#include "library.h"

struct sembuf sops[3];
void errExit(char * chartext )
{ 
    printf("%s\n",chartext);
    exit(EXIT_FAILURE); 
}
int main(int argc, char const *argv[])
{
    key_t key_sem = ftok("dontwait.c",'a'); if( key_sem <0 ) { perror("ERROR IN KEY\n");exit(EXIT_FAILURE);}
    int semid = semget(key_sem ,1 , IPC_CREAT|0666) ; if( semid <0 ) { perror("ERROR IN SEMGET\n");exit(EXIT_FAILURE);}
    sops[0].sem_num = 0;
    sops[0].sem_op = -1; // DECREMENTO di 1 il semaforo 0 sops[0].sem_flg = 0;
    sops[1].sem_num = 1;
    sops[1].sem_op = 2; // INCREMENTO di 2 il semaforo 1 */ sops[1].sem_flg = 0;
    sops[2].sem_num = 2;
    sops[2].sem_op = 0;           // ATTESA che il semaforo 2 valga 0 */
    sops[2].sem_flg = IPC_NOWAIT; /* operazione NON SOSPENSIVA: se l'operazione non può essere effettuata, NON attendere */
    if (semop(semid, sops, 3) == -1)
    {
        if (errno == EAGAIN) /* il semaforo 2 si sarebbe bloccato; invece grazie
        a IPC_NOWAIT ha restituito EAGAIN */
            printf("Operation would have blocked\n");
    
    }
    return 0 ; 
}
