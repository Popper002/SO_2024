#include "library.h"
/* SEND : GENERA IPC E LA CHIAVE E L'ID SONO CONVERTITI IN STRINGA E SPEDITI COME PARAMETRO AL
RCV */
char **ipc_id[100];
    int shm_id;
    key_t key_ ;
static void ipc_arg(char *argv[])
{
    char id[10]; 
    char key[10]; 
    sprintf(id , "%d",shm_id);
    sprintf(key, "%d",key_); 
    argv[1] = strdup(id); 
    argv[2] = strdup(key);
    argv[3] = NULL;
}
pid_t rcv()
{ 
    pid_t rcv ; 
    switch (rcv =fork())
    {
    case 0: 
            ipc_arg((char**)ipc_id);
            execvp("./rcv",(char**)ipc_id);
            perror("NOT PASS\n");
            exit(EXIT_FAILURE); 
        break;
    case -1: 
        perror("FORK ERROR\n");
        exit(EXIT_FAILURE); 
    default:
        return rcv; 
        break;
    }
}
int main(int argc, char const *argv[])
{
    key_ = ftok("send.c",'x');
    printf("KEY NUM IS %d\n" ,key_);
    if(key_ < 0 ){ perror("ERROR IN KEY GEN\n");exit(EXIT_FAILURE);}
    shm_id = shmget(key_, ARRAY_SIZE , IPC_CREAT|0666); 
    if(shm_id <0){perror("ERROR IN GET\n");exit(EXIT_FAILURE);} 
    printf(" %s SHM ID %d , sizeof(%d)",__FILE__ , shm_id , sizeof(shm_id));
    ipc_id[0]="./send";
    rcv(); 
    sleep(5);
    MyData *data; 
    data=(MyData* ) shmat(shm_id, NULL, 0);
    for( int i =0 ; i < 3 ;i++) 
    {
    printf("Il valore memorizzato è %d\n", data->array[i]);
    }
    

    return 0;
}
