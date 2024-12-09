#include "library.h"
#define SEC_STOP 4
#define SEC_RESTORE 4
#define SEC_SIM 60
#define IPC_KEY rand()%20000
int msg_id; 

struct msg send; 
struct msg rcv; 
void remove_ipc()
{ 
    msgctl(msg_id ,IPC_RMID,NULL); 
}
void signal_handler(int signum)
{
    if( signum == SIGUSR1)
    {
            fprintf(stdout,"SIGSTOP : STOPPING FOR %d SEC\n",SEC_STOP);
            sleep(SEC_STOP);
    }
    if(signum ==SIGUSR2)
    {
        fprintf(stdout ,"SIGCONT : RESTART PROC FOR ANOTHER %d SEC\n",SEC_RESTORE); 
        kill(getpid(),SIGCONT); 
    }
    if(signum ==SIGALRM)
    { 
        fprintf(stdout,"ALARM IS TIME TO STOP<TERM_REASON : TIMEOUT >\n"); 
        remove_ipc(); 
        exit(EXIT_SUCCESS); 

    }
}
int random_data()
{
    
    return rand() %1000;
}
int main(int argc, char const *argv[])
{   
    srand(time(NULL));
    signal(SIGUSR1 ,signal_handler); 
    signal(SIGUSR2,signal_handler); 
    signal(SIGALRM, signal_handler); 
    alarm(SEC_SIM);
    int  return_num ; 
     msg_id = msgget(IPC_KEY , IPC_CREAT | 0666); 
    if(msg_id < 0){fprintf(stderr,"ERROR IN MSGGET :ERR : %d \n",errno); exit(EXIT_FAILURE); }
    fprintf(stdout , " MSG_ID : %d < CREATED  > \n",msg_id); 


  
    while (1)
    {       
            send.msg_type =0; 
             rcv.msg_type =0; 
            return_num =random_data();
            #ifdef _PRINT_TEST
            fprintf(stdout,"ret_val %d\n",return_num);
            #endif
            sprintf(send.text , "%d",return_num); 
            msgsnd(msg_id , &send ,sizeof(send),0);// { fprintf(stderr , "ERROR IN SEND MSG : ERR: %d ",errno); exit(EXIT_FAILURE);}
            
            fprintf(stdout,"SEND COMPLEATE:THIS VALUE%s\n",send.text);
            kill(getpid(),SIGUSR1);
            kill(getpid(),SIGUSR2); 
            msgrcv(msg_id , &rcv , sizeof(rcv),0,0); 
            fprintf(stdout,"FETCH FROM QUEUED %d < THIS :%s >\n",msg_id,rcv.text);
            //bzero(&rcv,sizeof(rcv));       
                 
    }   
    
    
    return 0;
}
