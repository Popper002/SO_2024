#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/signal.h>
#include <sys/sem.h>
#include "../../src/util/my_sem_lib.h"

int semid; 
int paused = 0;

void sigint_handler(int sig) {
    if (paused) {
        printf("Resuming process...\n");
        paused = 0;
        sem_release(semid, 0, 1);
    } else {
        printf("Pausing process...\n");
        paused = 1;
        sem_reserve(semid, 0, 0); 
    }
}

int main() {
    key_t key;
    union semun arg;

    key = ftok("my_sem_file", 1);
    semid = semget(key, 1, IPC_CREAT | 0666);

    arg.val = 1;
    semctl(semid, 0, SETVAL, arg);

    signal(SIGINT, sigint_handler);

    while (1) {
        printf("Process running...\n");
        sleep(1);

        sem_reserve(semid, 0, 0);

          printf("Process resumed.\n");
    }

    return 0;
}
