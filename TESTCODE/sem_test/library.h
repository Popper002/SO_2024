#include <stdio.h>
#include<sys/ipc.h>
#include <unistd.h>
#include <string.h>
#include <sys/msg.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <sys/types.h>
#define macro_key 12


struct msg{
    long msg_type;
    char text[10]; 
};