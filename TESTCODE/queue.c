#include "library.h"
struct msg msg; 

int main(int argc, char const *argv[])
{
    
    srand(time(NULL));
    int id  = msgget(macro_key ,IPC_CREAT|0666); 
    if(id <= -1 ){ printf("problem in msgget \n"); exit(EXIT_FAILURE);}
    printf("QUEUE ID %d\n " , id ); 
    int randomic = rand()%20; 
    msg.msg_type =1; 
    sprintf(msg.text , "%d",randomic); 
    msgsnd(id , &msg ,sizeof(msg) -sizeof(long), 1); 
    printf("QUEUE%d\n",id); 
    printf("SEND THIS MESSAGE %s in LINE %ld",msg.text,msg.msg_type);
    return 0;
}
