#include "library.h"

struct data_rcv
{
    int data1;
    int num2; 
    int porcoDio; 
}; 
struct data_rcv *shared_data; 

int main(int argc, char const *argv[])
{
    void *rcv_ptr; 
    int id; 
    id = shmget(0x22 , sizeof(struct data_rcv) , IPC_CREAT|0666); 

    rcv_ptr = shmat(id,NULL,0);
    shared_data = (struct data_rcv*)rcv_ptr;
    printf("Valore: %d\nMessaggio: %d ,PORCODIO = %d , \n", shared_data->data1, shared_data->num2 , shared_data->porcoDio);
    return 0;
}
