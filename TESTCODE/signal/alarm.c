#include "library.h"

void signal_handle(int signum)
{
    if(signum == SIGALRM)
    { 
        printf("IS TIME TO KILL\n");
        kill(getpid(),SIGINT);
    }
}
int main(int argc, char const *argv[])
{
    struct sigaction sa; 
    int signum; 
    sa.sa_handler = &signal_handle;
    printf("ALARM : PROC %d\n",getpid()); 
    alarm(10);

    return 0;
}
