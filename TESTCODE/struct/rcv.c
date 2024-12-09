#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

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

    while (1) {
        // Ricezione dei valori dalla message queue
        msgrcv(msg_id, &message, sizeof(message.data), 1, 0);

        // Stampiamo i valori ricevuti
        printf("Valori ricevuti: dato1=%d, dato2=%d, dato3=%d\n", 
               message.data.dato1, message.data.dato2, message.data.dato3);
    }

    return 0;
}
