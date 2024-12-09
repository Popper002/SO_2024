#include "library.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

struct data_send
{
    int data1;
    int num2;
    int porcoDio;
};

int main(int argc, char const *argv[])
{
    int shid;
    struct data_send *send;

    // Alloca memoria per la struttura
    send = (struct data_send *)malloc(sizeof(struct data_send));
    if (send == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    // Assegna valori alla struttura
    send->data1 = 1;
    send->num2 = 21;
    send->porcoDio = 2112231;

    // Ottieni l'ID della memoria condivisa
    shid = shmget(0x22, sizeof(struct data_send), IPC_CREAT | 0666);
    if (shid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // Attacca la memoria condivisa al processo
    void *ptr = shmat(shid, NULL, 0);
    if (ptr == (void *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    // Copia i dati dalla struttura alla memoria condivisa
    memcpy(ptr, send, sizeof(struct data_send));

    // Rilascia la memoria condivisa
    shmdt(ptr);

    // Libera la memoria allocata per la struttura
    free(send);

    return 0;
}
