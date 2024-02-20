#include "library.h"

/* esempio 2 , qua invece aspettiamo che il semaforo sia zero per far partire una bomba di processi a stampa a video ,molto utile per il progetto
poichè i processi generati dal master devono attendere che esso finisca di settare tutto prima di partire a simulare */
#define NUM_PROC 2
#define KEY 0x21
#define ID_OP1 1
#define ID_OP2 2
struct sembuf sem_op;
int array_pid[NUM_PROC];
static void printer()
{
    printf("PROC %d SAY PIPPOO\n", getpid());
}
int main(int argc, char const *argv[])
{
    int sem_id,i; 
    pid_t proc;
    key_t key_sem;
    key_sem = ftok("waitzero.c",'a'); 
    if( key_sem < 0 ) {perror("ERROR IN KEY\n"); exit(EXIT_FAILURE);} 
    if( sem_id = semget(key_sem , 1 , IPC_CREAT|0666) < 0 ) { perror("PROBLEM IN SEMGET\n"); exit(EXIT_FAILURE);} 
    semctl(sem_id , 0 , SETVAL , 0 ); 
    for( i =0 ;i < NUM_PROC ; i++ )
    {
        if( proc = fork() < 0 ) { perror("ERROR IN FORK\n"); exit(EXIT_FAILURE);}; 
        sem_op.sem_num = 1; 
        sem_op.sem_op = 0;
        printf("SEM %d PROC %d WAIT\n",sem_id , getpid() ); 
    }

    sem_op.sem_num=1; 
    sem_op.sem_op = -1; 
    printer(); 
    return 0;
}


