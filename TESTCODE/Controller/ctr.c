#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#define MAX_COMMAND_LENGTH 100

pid_t inhibitor_pid;

void signal_handler(int signum) {
    if (signum == SIGSTOP) {
        printf("Processo inibitore in pausa. Premi 'r' per riprendere.\n");
    } else if (signum == SIGCONT) {
        printf("Processo inibitore ripreso.\n");
    }
}

void start_inhibitor_process() {
    // Avvia il processo inibitore
    inhibitor_pid = fork();
    if (inhibitor_pid == 0) {
        execvp("./printer",NULL);
        exit(EXIT_FAILURE);
    } else if (inhibitor_pid < 0) {
        perror("fork");
    }
}

void stop_inhibitor_process() {
    // Ferma il processo inibitore
    if (inhibitor_pid > 0) {
        kill(inhibitor_pid, SIGSTOP);
    } else {
        printf("Processo inibitore non ancora avviato.\n");
    }
}

void resume_inhibitor_process() {
    // Riprende il processo inibitore
    if (inhibitor_pid > 0) {
        kill(inhibitor_pid, SIGCONT);
    } else {
        printf("Processo inibitore non ancora avviato.\n");
    }
}

int main() {
    char command[MAX_COMMAND_LENGTH];

    // Imposta il gestore del segnale per SIGSTOP e SIGCONT
    signal(SIGSTOP, signal_handler);
    signal(SIGCONT, signal_handler);

    while (1) {
        // Stampa il prompt della CLI
        fprintf(stdout,"Comando (start/stop/resume/quit): ");
      

        // Legge il comando dall'utente
        if (fgets(command, sizeof(command), stdin) == NULL) {
            break;
        }

        // Rimuove il carattere newline finale se presente
        command[strcspn(command, "\n")] = '\0';

        // Esegue il comando corrispondente
        if (strcmp(command, "start") == 0) {
            start_inhibitor_process();
        } else if (strcmp(command, "stop") == 0) {
            stop_inhibitor_process();
        } else if (strcmp(command, "resume") == 0) {
            resume_inhibitor_process();
        } else if (strcmp(command, "quit") == 0) {
            break;
        } else {
            printf("Comando non riconosciuto. I comandi validi sono 'start', 'stop', 'resume' e 'quit'.\n");
        }
    }

    // Termina il programma
    return 0;
}
