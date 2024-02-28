#include "library.h"
pid_t inib()
{
    switch (fork())
    {
    case 0: 
        execvp("./rcv",NULL);
        fprintf(stderr, "%s", strerror(errno));
        exit(EXIT_FAILURE);
        break;
    
    default:
        return getpid();
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
    switch (fork())
    {
    case -1:
        fprintf(stderr, "%s", strerror(errno));
        exit(EXIT_FAILURE);
    case 0:
        printf("INIT SEM OPERATION\n");
                fflush(stdout);

        op.sem_flg = 0;
        op.sem_num = 0;
        op.sem_op = -1;
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
         
        op.sem_flg = 0;
        op.sem_num = 0;
        op.sem_op = 1; // Incrementa il semaforo
        break;
    }
    semctl(sem_id, 1, IPC_RMID);
    return 0;
}
