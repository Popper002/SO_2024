#include "library.h"
#define SET_SECOND 30
#define TIME_SLEEP 4
char *string_reason; 
enum term_reason
{ 
    TIMEOUT = 0, 
    EXPLODE =1, 
    MEMEORY_FULL =2
}; 
void alarm_handle(int signum)
{
    if(signum == SIGALRM)
    {
         fprintf(stdout,"ALARM !!!\r\n");
         fprintf(stdout,"TEARM_REASON : TIMEOUT < CODE : %d>\n",TIMEOUT); 
         exit(EXIT_SUCCESS);  
    }
    if(signum ==SIGUSR1)
    { 
            fprintf(stdout,"STOPPPING RIGHT NOW\n"); 
            sleep(TIME_SLEEP);
            
    }
    if (signum==SIGUSR2)
    {
        fprintf(stdout,"RESTART NOW \n");
    }
    if(signum ==SIGTERM)
    {
        fprintf(stdout , "NAH <<<< I M CONTINUEEE >>>>STOPPING ONLY WITH TIMER\n"); 
    }
}
void printerr()
{ 
    fprintf(stdout , " HELLO FROM PRINTER FUNC \n") ; 

} 
int main(int argc, char const *argv[])
{
    signal(SIGALRM ,alarm_handle); //setup the signal 
    signal(SIGUSR1,alarm_handle);
    signal(SIGUSR2,alarm_handle);
    signal(SIGTERM , alarm_handle); 
    alarm(SET_SECOND); 
    int time ; 
    time = SET_SECOND; 



    while(1)
    {
        
         kill(getpid(),SIGUSR1);
         kill(getpid(),SIGUSR2); //Send SIGUSR 2  SIGNAL 
         printerr(); 
         printf("TIME REST %d\r\n",time);
         fflush(stdout);
         sleep(1); 
         time --; 
         kill(getpid(),SIGTERM); 
         
    }
    return 0;
}
