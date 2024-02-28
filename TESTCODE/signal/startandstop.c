#include "library.h"
#define _GNU_SOURCE  
/* 
void sigchld_handler(int sig) {
  pid_t child_pid;
  while ((child_pid = waitpid(-1, NULL, WNOHANG)) > 0) {
    write(STDOUT_FILENO,"Processo figlio terminato.\n", 28);
    kill(child_pid, SIGKILL);
  }
  if( sig == SIGINT)
  { 
    write(STDOUT_FILENO , "\nKILLED\n" ,9); 
    kill(child_pid,SIGKILL); 
  }
}

int main() {
  pid_t pid = getpid();
  printf("Processo padre: %d\n", pid);

  // Imposta il gestore per il segnale SIGCHLD.
  struct sigaction act;
  bzero(&act , sizeof(act)); 
  act.sa_handler = sigchld_handler;
  //sigemptyset(&act.sa_mask);
  act.sa_flags = 0;
  sigaction(SIGCHLD, &act, NULL);
  sigaction(SIGINT , &act,NULL); 
  
  // Crea un processo figlio.
  pid_t child_pid = fork();
  if (child_pid == 0) {
    // Processo figlio
    printf("Processo figlio: %d\n", getpid());
    int i = 0;
    while (i < 10) {
      sleep(1);
      i++;
    }
    exit(0);
  } else if (child_pid > 0) {
    // Processo padre
    printf("Creato processo figlio: %d\n", child_pid);
    while (1) {
      sleep(1);
    }
  } else {
    perror("Errore nella creazione del processo figlio");
    return 1;
  }
    exit(EXIT_SUCCESS);
  return 0;
}
*/ 
#define NUM_PROC 2
pid_t gen_child()
{ 
    pid_t pid = fork();
    if (pid == 0) {
        #ifdef DEBUG_PRINT
        printf("PROC_TEST: PROC PID %d GENERATE\n", getpid());
        #endif
    } else if (pid < 0) {
        // Handle fork() failure (e.g., log or break)
        perror("fork() failed");
    }
    return pid;
}
int fill_array(pid_t array[], int max_procs) {
    int count = 0;
    for (int i = 0; i < max_procs; i++) {
        pid_t child_pid = gen_child();
        if (child_pid > 0) {
            // Store successful child PID in the array
            array[count] = child_pid;
            count++;
        } else if (child_pid == 0) {
            // Handle child process execution
            // (You might want to add logic here depending on your use case)
        }
    }
    return count; // Return actual number of PIDs stored
}
void handle_signal(int signum)
{
    if( signum ==SIGINT)
    {
        write(STDOUT_FILENO,"\nRECEIVED START KILLING\n",25);   

    }
    else if(signum == SIGSTOP)
    {
        write(STDOUT_FILENO,"\nRECEIVED STOP SIGNAL\n",23);
    }
    else if(signum ==SIGCONT)
    {
        write(STDOUT_FILENO ,"\nRECEIVED RESTART SIGNAL\n",26);
    }
}
int main(int argc, char const *argv[])
{
    struct sigaction sa; 
    bzero(&sa, sizeof(sa));
    sa.sa_handler = &handle_signal;
     int pid_array[100]; // Adjust size as needed
    int num_procs = fill_array(pid_array, NUM_PROC); // Specify desired number of child processes
    sigaction(SIGSTOP,&sa,NULL);
    sigaction(SIGCONT , &sa , NULL); 
    //sigaction(SIGINT ,&sa, NULL); 

    for (int i = 0; i < num_procs; i++) {
        printf("Generated child PID: %d\n", pid_array[i]);
    }
 /*
       for(int i =0 ; i<num_procs ; i++)
    {
        write(STDOUT_FILENO , "RESTART PROC\n",14);
        signal(pid_array[i],SIGCONT);
        kill(pid_array[i],SIGCONT);
    }
  
    // Wait for child processes to finish (assuming desired behavior)
    for (int i = 0; i < num_procs; i++) {
        waitpid(pid_array[i], NULL, 0);
    }
    */
    return 0;
}
