

#include "library.h"
#define STOP_PATH "./stop"
#define NUM_PROC 10
int *pid_collect;
pid_t generator()
{   
    pid_t pid;
    switch (pid = fork())
    {
    case -1: 
        fprintf(stderr, "ERROR IN FORK\n"); 
        exit(EXIT_FAILURE); 
        break;
    case 0: 
        execvp(STOP_PATH,NULL); 
        fprintf(stderr,"ERROR IN EXECVP\n"); 
        exit(EXIT_FAILURE); 
    default:
            return pid;
        break;
    }
}
void handle(int signum)
{
    if(signum==SIGALRM)
    {
        fprintf(stdout,"IT'S TIME TO STOP\n");
        for(int i=0 ;i<NUM_PROC;i++)
        {
            kill(pid_collect[i],SIGKILL);
            fprintf(stdout,"PROC %d KILLED\n",pid_collect[i]); 

        }
        fprintf(stdout,"SIMULATION COMPLEAT BYE\n");
        exit(EXIT_SUCCESS);
    }
}
int main(int argc, char const *argv[])
{
    
    int sim_time = 5; 
    int time;
    int sem_unlock;
     struct sembuf seop;
    sem_unlock = semget(0x21 , 1, IPC_CREAT |0666); 
    if(sem_unlock < 0 ){
         fprintf(stderr , "ERROR INIT SEM\n"); 
         exit(EXIT_FAILURE);   
    }
    pid_collect = (int * ) malloc(sizeof(pid_t)*NUM_PROC);
    signal(SIGALRM , handle); 
    /*
    int sem_id = semget(SEM_KEY , IPC_EXCL|IPC_CREAT|0666); 
    if (sem_id == -1) {
        // Se il semaforo esiste già, ottieni il suo identificatore
        sem_id = semget(SEM_KEY, 1, 0);
    }else{
        semctl(sem_id , 0,SETVAL,NUM_PROC); 
    }
    */
    semctl(sem_unlock,0,SETVAL,1);

    for(int i =0 ; i< NUM_PROC ; i++)
    { 
            pid_collect[i] = generator(); 
    }
    sleep(1);
    fprintf(stdout,"MASTER COMPLEATE GENERATION NOW WAIT\n");
    for(time= 10 ; time ; time--)
    {
        fprintf(stdout,"SIMULATION START IN %d\n",time);
        sleep(1);  
    }
    alarm(sim_time); 
    fprintf(stdout,"SIMULATION START NOW\n");
      for(int i =0 ; i< NUM_PROC ; i++)
    { 
           kill(pid_collect[i],SIGCONT);
           fprintf(stdout,"PROC %d RAISED\n",pid_collect[i]);
    }
    seop.sem_flg=0;
    seop.sem_num=0;
    seop.sem_op=-1; 
    semop(sem_unlock,&seop,1); 

    fprintf(stdout,"SLEEP FOR 3 SEC\n");
    sleep(3); 
 
    seop.sem_flg=0;
    seop.sem_num=0;
    seop.sem_op=1; 
    semop(sem_unlock,&seop,1); 
    while (1)
    {
            
    }
    
    return 0;
}
