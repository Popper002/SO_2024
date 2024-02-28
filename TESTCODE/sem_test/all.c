#include "library.h"

#define NUM_FORK 6
#define SEM_INIT_VAL 3
#define TEST_ERROR    if (errno) {fprintf(stderr, \
					   "%s:%d: PID=%5d: Error %d (%s)\n",\
					   __FILE__,\
					   __LINE__,\
					   getpid(),\
					   errno,\
					   strerror(errno));}




int main(int argc, char const *argv[])
{
    pid_t child_pid;
    int status,s_id; 
    time_t start,end; 
    struct sembuf operation; 

    s_id = semget(IPC_PRIVATE , 1 , 0600);
    TEST_ERROR;
    semctl(s_id , 0 , SETVAL , SEM_INIT_VAL); 
    TEST_ERROR;
    operation.sem_num = 0;
    operation.sem_flg = 0; 
    start  = time(NULL);
    
    for(int i =0 ; i< NUM_FORK ; i++)
    { 
        switch (fork())
        {
        case -1: 
            TEST_ERROR; 
            exit(EXIT_FAILURE); 
            break;
        case 0: 
                operation.sem_op = -1 ; 
                semop(s_id,&operation, 1);
                TEST_ERROR;
                execvp("./exe",NULL);
             
                
        default:
            break;
        }
    }
                operation.sem_op =1;
                semop(s_id ,&operation,1); 
                TEST_ERROR;
                exit(0);
    kill(getpid(),SIGSTOP);
    printf("STOPPO CHILD\n");
    sleep(5);
    kill(getpid(),SIGCONT);
  
    end = time(NULL); 
    fprintf(stderr,"Total time: %ld (sec)\n", end-start);
    semctl(s_id , 0 , IPC_RMID); 
    exit(EXIT_SUCCESS);
    return 0;
}
