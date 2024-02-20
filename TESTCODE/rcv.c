#include "library.h"
struct msg rcv; 

int main(int argc, char const *argv[])
{
    int id_get = msgget(macro_key , IPC_CREAT |0666); 
    if(id_get <=-1 )
    {
         perror("ERRORE IN GET ");
         exit(EXIT_FAILURE);
    }
    rcv.msg_type=1; 
    msgrcv(id_get,&rcv,sizeof(rcv)-sizeof(long),1 ,0); 
    printf("PRINT DATA FROM QUEUE %d <DATA %s >\n",id_get , rcv.text);
    
    return 0;
}
