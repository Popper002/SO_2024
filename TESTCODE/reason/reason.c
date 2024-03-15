#include "library.h"


enum term_reason
{
    TIMEOUT,
    DEAD,
    NO_REASOURCE, 
    NO_SPACE,

};

enum term_reason termination ; 


int reaoson(enum term_reason termination)
{ 
    switch (termination)
    {
    case TIMEOUT: 
            write(STDOUT_FILENO , "TERMINATION : TIMEOUT TIME FINESHED\n",37 );
            break;
    case DEAD: 
        write(STDERR_FILENO , "TERMINATION : DEAD - SOMEBODY DEAD\n",36);
        exit(EXIT_FAILURE);
        break;
    case NO_REASOURCE: 
        write(STDERR_FILENO,"TERMINATION: NO_RESOURCE - THE RESOURCE IS NOT AVAIBLE\n",56); 
        exit(EXIT_FAILURE); 
        break;
    case NO_SPACE: 
        write(STDERR_FILENO , "TERMINATION : NO_SPACE - MEMORY NOT ALLOCATED OR SPACE IS NOT ENOUGH\n",70);
        exit(EXIT_FAILURE); 
        break;
    default:
        break;
    }
    return termination; 
}
void alarm_handle(int signum)
{ 
    switch (signum)
    {
    case SIGALRM: 
            reaoson(TIMEOUT);
        break;
    
    default:
        break;
    }
}
int main(int argc, char const *argv[])
{
    signal(SIGALRM, alarm_handle); 
    /* NO_SPACE 
    int no_space = (char )malloc(sizeof(-1)); 
    if(no_space<0 )
    { 
        reaoson(NO_SPACE); 
    }
    */ 
    /* FORK ERROR 
    pid_t pid = fork(); 
    pid = -1; 
    if(pid ==-1)
    {
         int log_reason ; 
         log_reason = reaoson(DEAD); 
    }
    */ 
   /* 
   key_t key; 
   int queue; 
   key = 0x01; 
    if(msgsnd(queue , &queue , sizeof(NULL),0) <0) {

        int log_reason ; 
        log_reason = reaoson(NO_REASOURCE);
    }
    */ 
   /* time out 
   */

    alarm(10); 
    return 0;
}
