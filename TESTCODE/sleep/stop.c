#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <signal.h>

// Chiave per il semaforo System V
#define SEM_KEY 1234

// Funzione per mettere in pausa il processo inibitore
void stopInibitore(int sem_id) {
    struct sembuf sop;
    sop.sem_num = 0;
    sop.sem_op = -1;
    sop.sem_flg = 0;

    semop(sem_id, &sop, 1);
    printf("Processo inibitore in pausa...\n");
}

// Funzione per far ripartire il processo inibitore
void resumeInibitore(int sem_id) {
    struct sembuf sop;
    sop.sem_num = 0;
    sop.sem_op = 1;
    sop.sem_flg = 0;

    semop(sem_id, &sop, 1);
    printf("Ripresa dell'esecuzione del processo inibitore...\n");
}

// Gestore del segnale SIGUSR1 per il controllo del processo inibitore
void sigusr1_handler(int signum) {
    int sem_id = semget(SEM_KEY, 1, IPC_CREAT | 0666);
    if (sem_id == -1) {
        perror("semget");
        exit(EXIT_FAILURE);
    }

    struct sembuf sop;
    sop.sem_num = 0;
    sop.sem_flg = 0;

    if (semctl(sem_id, 0, GETVAL) > 0)
        stopInibitore(sem_id);
    else
        resumeInibitore(sem_id);
}

// Funzione per l'esecuzione del processo inibitore
void runInibitore() {
    while (1) {
        // Simulazione dell'attività del processo inibitore
        printf("Il processo inibitore è attivo...\n");
        sleep(1);
    }
}

int main() {
    char scelta;
    printf("Vuoi attivare il processo inibitore? (s/n): ");
    scanf(" %c", &scelta);

    // Se l'utente sceglie di attivare il processo inibitore
    if (scelta == 's') {
        // Installa il gestore per il segnale SIGUSR1
        signal(SIGUSR1, sigusr1_handler);

        // Creazione del semaforo System V
        int sem_id = semget(SEM_KEY, 1, IPC_CREAT | 0666);
        if (sem_id == -1) {
            perror("semget");
            exit(EXIT_FAILURE);
        }

        // Inizializzazione del semaforo a 1
        semctl(sem_id, 0, SETVAL, 1);

        // Avvia il processo inibitore
        runInibitore();
    } else {
        printf("Il processo inibitore non è attivo.\n");
    }

    return 0;
}