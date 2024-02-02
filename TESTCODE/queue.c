#include <sys/ipc.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define macro_key 12

int main(int argc, char const *argv[])
{
    
    int id  = msgget(macro_key ,IPC_CREAT|0666); 
    printf("QUEUE%d\n",id); 
    return 0;
}
