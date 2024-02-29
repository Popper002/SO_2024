#include "library.h"
pid_t inib()
{
    pid_t ret_pid;
    switch (ret_pid = fork())
    {
    case 0: 
        execvp("./rcv",NULL);
        fprintf(stderr, "%s", strerror(errno));
        exit(EXIT_FAILURE);
        break;
    
    default:
        return ret_pid;
        break;
    }
}
int main(int argc, char const *argv[])
{
    struct sembuf op;
    sem_id = semget(0x22, 1, IPC_CREAT | 0600);
    semctl(sem_id, 1, SETVAL, 0);
    pid_t inibitore; 
    inibitore = inib();
    for(int i =0;  i <NUM_PROC; i++ ){      
        printf("iterzaione num: %d\n", i);
        fflush(stdout);
    switch (fork())
    {
    case -1:
        fprintf(stderr, "%s", strerror(errno));
        exit(EXIT_FAILURE);
    case 0:
        printf("INIT SEM OPERATION\n");
                fflush(stdout);
//sem_reserve
        op.sem_flg = 0;
        op.sem_num = 0;
        op.sem_op = -1;
//sem_reserve
        semop(sem_id, &op, 1);
        printf("BEFORE EXECVP\n");
                fflush(stdout);

        execvp("./all", NULL);
        fprintf(stderr, "%s", strerror(errno));
        exit(EXIT_FAILURE);
        break;

    default: //padre
        printf("DOING THINGS [PID %d ]\n", getpid());
                fflush(stdout);

        printf("WAIT TO UNLOCK MY CHILD'S\n");
             fflush(stdout);

        printf("AFTER THIS I'M OPEN THE DOOR\n");
        fflush(stdout);
       
        break;
    }
}
    
    semctl(sem_id, 1, IPC_RMID);
    return 0;
}
