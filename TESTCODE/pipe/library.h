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
#include <sys/shm.h>
#include <fcntl.h>            
#include <unistd.h>
#define ARRAY_SIZE 10
#define NUM_PROC 10 
typedef struct {
  int array[ARRAY_SIZE];
} MyData;

