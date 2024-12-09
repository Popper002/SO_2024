#include <stdio.h>
#include "library.h"

int main(int argc, char const *argv[]) {
    int rcv_id = msgget(KEY_, IPC_CREAT | 0666); // La stessa coda dei messaggi

    if (rcv_id < 0) {
        fprintf(stderr, "ERROR IN MSGGET <ERRNO:%s>\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    MyData rcv;
    printf("+----------------------+----------------------+----------------------+\n");
    printf("|       Tipo 0         |        Tipo 1        |        Tipo 2        |\n");
    printf("+----------------------+----------------------+----------------------+\n");

    while (1) {
        // Leggi un messaggio dalla coda dei messaggi con tipo 0
        if (msgrcv(rcv_id, &rcv, sizeof(rcv) - sizeof(long), 0, IPC_NOWAIT) > 0) {
            printf("| %-20s", rcv.array);
        } else {
            printf("| %-20s", "N/A");
        }

        // Leggi un messaggio dalla coda dei messaggi con tipo 1
        if (msgrcv(rcv_id, &rcv, sizeof(rcv) - sizeof(long), 1, IPC_NOWAIT) > 0) {
            printf("| %-20s", rcv.array);
        } else {
            printf("| %-20s", "N/A");
        }

        // Leggi un messaggio dalla coda dei messaggi con tipo 2
        if (msgrcv(rcv_id, &rcv, sizeof(rcv) - sizeof(long), 2, IPC_NOWAIT) > 0) {
            printf("| %-20s", rcv.array);
        } else {
            printf("| %-20s", "N/A");
        }
        printf("|\n");
        printf("+----------------------+----------------------+----------------------+\n");
        // Aggiungi qui la logica per gestire i diversi tipi di messaggio se necessario
    }

    return 0;
}
