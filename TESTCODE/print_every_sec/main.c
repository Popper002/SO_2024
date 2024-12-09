#include "library.h"
#define TIME_EXE 10
#define PROC_NUM 3



int main(int argc, char const *argv[])
{
    unsigned int time;
    time = 2;
    pid_t pid_child;
    while( time -- )
    {
        for( int i =0 ; i<PROC_NUM;i++)
        {
        if(pid_child = fork() < -1)
        {
            fprintf(stderr,"%s",strerror(errno));
            exit(EXIT_FAILURE);
        }   
        pid_t getpid_child = getpid();
        pid_t parent_pid= getppid(); 
        printf("PID=%6d:  PPID=%6d,  i=%d, fork_value=%d\n",
		       getpid_child, parent_pid, i, pid_child);
        sleep(1);

     }
    }

    while (wait(NULL)!= -1)
    {
        printf("PROC=%d , TERMINATED\n",getpid());
    }
    
    return 0;
}
