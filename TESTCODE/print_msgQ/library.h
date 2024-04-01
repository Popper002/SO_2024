#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif 
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
#define ARRAY_SIZE 30
#define KEY_ 0x11
typedef struct {
  char array[ARRAY_SIZE];
  long type;
} MyData;

