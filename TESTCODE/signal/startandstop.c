#include "../../src/util/my_sem_lib.h"
#include <stdio.h>
#include <sys/sem.h>
#include <sys/signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int semid;
int paused = 0;
pid_t labrat_pid; // PID of the labrat process

void sigint_handler() {
    if (paused) {
        printf("Resuming labrat process...\n");
        paused = 0;
        kill(labrat_pid, SIGCONT); // Send SIGCONT to resume labrat
        sem_release(semid, 0, 1);  // Release semaphore for labrat execution
    } else {
        printf("Pausing labrat process...\n");
        paused = 1;
        kill(labrat_pid, SIGSTOP);  // Send SIGSTOP to pause labrat
        sem_reserve(semid, 0, 0);   // Acquire semaphore to block main process
    }
}

int main() {
    key_t key;
    int status;

    // Create semaphore set with 1 semaphore
    key = ftok("my_sem_file", 1);
    semid = semget(key, 1, IPC_CREAT | 0666);
    if (semid == -1) {
        perror("semget");
        exit(1);
    }

    // Initialize semaphore with value 1 (not strictly necessary here)
    semctl(semid, 0, SETVAL, NULL);

    // Set up signal handler for SIGINT
    signal(SIGINT, sigint_handler);

    // Fork a child process to execute labrat
    labrat_pid = fork();
    if (labrat_pid == -1) {
        perror("fork");
        exit(1);
    }

    if (labrat_pid == 0) {
        // Child process (labrat)
        execve("labrat", NULL,NULL);
        perror("execvp"); // Handle errors if execvp fails
        exit(1);
    }

    while (1) {
        printf("Main process running...\n");

        // You can add code here for the main process to do something
        // while labrat is paused (if applicable)
    }

    // Wait for labrat process to finish (optional)
    waitpid(labrat_pid, &status, 0);
    if (WIFEXITED(status)) {
        printf("Labrat process exited with status %d\n", WEXITSTATUS(status));
    } else {
        printf("Labrat process terminated abnormally\n");
    }

    // Clean up resources (e.g., remove semaphore)
    semctl(semid, 0, IPC_RMID, NULL);

    return 0;
}
