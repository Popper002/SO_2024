#include "library.h"

int main(int argc, char const *argv[])
{
    struct sembuf op;
    sem_id = semget(0x22, 1, IPC_CREAT | 0600);
    semctl(sem_id, 1, SETVAL, 1);
    switch (fork())
    {
    case -1:
        fprintf(stderr, "%s", strerror(errno));
        exit(EXIT_FAILURE);
    case 0:
        printf("INIT SEM OPERATION\n");
        op.sem_flg = 0;
        op.sem_num = 0;
        op.sem_op = -1;
        semop(sem_id, &op, 1);
        printf("BEFORE EXECVP\n");
        execvp("./all", NULL);
        fprintf(stderr, "%s", strerror(errno));
        exit(EXIT_FAILURE);
        break;

    default: //padre
        printf("DOING THINGS [PID %d ]\n", getpid());
        printf("WAIT TO UNLOCK MY CHILD'S\n");
        printf("AFTER THIS I'M OPEN THE DOOR\n");
        op.sem_flg = 0;
        op.sem_num = 0;
        op.sem_op = 1; // Incrementa il semaforo
        semop(sem_id, &op, 1);
        break;
    }

    semctl(sem_id, 1, IPC_RMID);
    return 0;
}
