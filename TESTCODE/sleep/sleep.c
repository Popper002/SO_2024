#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/signal.h>
#include <math.h>


long sleep_time()
{   
        return rand() %100000000 ; 
}
void alarm_handle(int signum )
{
    if(signum == SIGALRM)
    {
         write(STDOUT_FILENO, "TIME TO STOP\n",14);
         kill(getpid(),SIGTERM);
         exit(EXIT_SUCCESS); 
    }
}
int main(int argc, char const *argv[])
{
    struct timespec time;
    time.tv_nsec = 4000000 *60;
    signal(SIGALRM , alarm_handle); 
    alarm(10); 
    while (1)
    {
    
       nanosleep( &time, NULL); 
       printf("WAKEUP\n"); 

       printf("CIAO\n");
    }
    
    return 0;
}
