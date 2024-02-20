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
#include <sys/shm.h>
#define ARRAY_SIZE 10
typedef struct {
  int array[ARRAY_SIZE];
} MyData;

