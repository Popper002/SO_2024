#include <stdio.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <string.h>
#include <sys/msg.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <signal.h>
#include <sys/types.h>
#define SEM_KEY 0x22


void semaphore_down(int sem_id);
void semaphore_up(int sem_id);

struct msg
{
    long msg_type; 
    char text[10];  
}; 