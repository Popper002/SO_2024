#include "library.h"


// Funzione per eseguire l'operazione down sul semaforo
void semaphore_down(int sem_id) {
    struct sembuf sops = {0, -1, 0};
    semop(sem_id, &sops, 1);
}

// Funzione per eseguire l'operazione up sul semaforo
void semaphore_up(int sem_id) {
    struct sembuf sops = {0, 1, 0};
    semop(sem_id, &sops, 1);
}

int main(int argc, char const *argv[])
{   
    struct sembuf sop;
    /*
    int sem_id = semget(SEM_KEY , 1,IPC_EXCL|IPC_CREAT |0666);
    if (sem_id == -1) {
        // Se il semaforo esiste già, ottieni il suo identificatore
        sem_id = semget(SEM_KEY, 1, 0);
    }
    */ 
    int sem_lock;
    sem_lock = semget(0x21 , 1, IPC_CREAT |0666); 
    if(sem_lock < 0 ){
         fprintf(stderr , "ERROR INIT SEM\n"); 
         exit(EXIT_FAILURE); 
         
    }

    fprintf(stdout,"ATOM %d CREATED\n",getpid()); 
    fprintf(stdout,"ATOM %d STOP\n",getpid()); 
    kill( getpid(),SIGSTOP); 
    fprintf(stdout,"RECEIVED RESTART SIGNAL BUT SEM BLOCK ME\n");
    sop.sem_num=0;
    sop.sem_flg=0;
    sop.sem_op=0; 
    semop(sem_lock,&sop,1); 
    fprintf(stdout,"OK NOW CAN I START\n");
    sleep(3);
    while (1)
    {
        fprintf(stdout,"Ciao FROM PID %d\n",getpid());
    }
    
    return 0;
}
