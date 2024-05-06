#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

#define NUM_PROC 5 // Definisci il numero di processi
/*Test_Code pipe chapter 1 
*  Abbiamo una pipe su qui scirviamo sul fd[1] dei valori randomici che verranno letti dai processi figli 
*  Unico bug attuale è che i valori scritto sono molto spesso uguali probabilmente perchè srand non ha il tempo di aggiornare il seads quindi adrebbe modificato usando un seads
*  da CLOCK_REALTIME 
*/ 
static int fd[2];
static int pipe_ret; 
static char buffer[100]; 
static int num;
static int num_bytes; 

void handler_signal(int signum) {
    switch (signum) {
        case SIGPIPE:
            write(STDOUT_FILENO, "\t\tSIGPIPE RECEIVED\n", 20);
            write(STDOUT_FILENO, "\t\tno read end is open???\n", 26);
            write(STDOUT_FILENO, "\t\tno users have the read fd open???\n", 37);
            write(STDOUT_FILENO, "\t\t", 3);
            break;
        default:
            break;
    }
}

int main(int argc, char const *argv[]) {
    srand(time(NULL)); 
    signal(SIGPIPE, handler_signal);
    num = rand() % 100; 
    pipe_ret = pipe(fd);
    if (pipe_ret < 0) { 
        fprintf(stderr, "ERROR OPENING PIPE SEGMENT\n");
        exit(EXIT_FAILURE); 
    }
    struct timespec time; 
    time.tv_nsec =rand()%10;
    for (int i = 0; i < NUM_PROC; i++) {
        pid_t child;
        switch (child = fork()) {
            case -1: 
                fprintf(stderr, "ERROR FORK\n"); 
                exit(EXIT_FAILURE);
                break;
            case 0: // Sezione del figlio
                close(fd[1]); // Chiudi il lato di scrittura del pipe nel processo figlio
                char read_fd_content[100]; // Buffer per leggere il contenuto di fd[0]
                int read_bytes = read(fd[0], read_fd_content, sizeof(read_fd_content));
                if (read_bytes > 0) {
                    read_fd_content[read_bytes] = '\0'; // Assicura che il buffer sia terminato correttamente
                    fprintf(stdout, "PROC CHILD THIS IS FD CONTENT: %s\n", read_fd_content);
                } else if (read_bytes == 0) {
                    fprintf(stdout, "PROC CHILD: Nothing read from pipe\n");
                } else {
                    fprintf(stderr, "PROC CHILD: Error reading from pipe\n");
                    exit(EXIT_FAILURE);
                }
                close(fd[0]); // Chiudi il lato di lettura del pipe nel processo figlio
                exit(EXIT_SUCCESS);
                break;
            default: // Sezione del padre
                // Scrivi nel pipe
                sprintf(buffer, "%d", num);
                num_bytes = write(fd[1], buffer, sizeof(buffer));
                fprintf(stdout, "NUM BYTES WRITTED %d\n", num_bytes);
                break;
        }
        
    }

    close(fd[0]); // Chiudi il lato di lettura del pipe nel processo padre
    close(fd[1]); // Chiudi il lato di scrittura del pipe nel processo padre

    return 0;
}
