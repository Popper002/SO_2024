#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#define INTERVAL 2

// Struttura per i messaggi nella message queue
struct msg_buffer {
    long msg_type;
    struct data {
        int dato1;
        int dato2;
        int dato3;
        // Aggiungi qui altri campi dati necessari
    } data;
};

int main() {
    struct msg_buffer message;
    key_t key = ftok("msg_queue", 'a');
    int msg_id = msgget(key, 0666 | IPC_CREAT);

    srand(time(NULL));

    while (1) {
        // Aggiornamento dei valori
        message.data.dato1 += rand() % 10 + 1; // Incremento casuale
        message.data.dato2 += rand() % 10 + 1; // Incremento casuale
        message.data.dato3 += rand() % 10 + 1; // Incremento casuale
        // Invio dei valori aggiornati tramite message queue
        message.msg_type = 1;
        msgsnd(msg_id, &message, sizeof(message.data), 0);

        sleep(INTERVAL);
    }

    return 0;
}
