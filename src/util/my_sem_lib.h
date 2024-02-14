#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>

union semun
{
  int val;
  struct semid_ds *buf;
  unsigned short *array;
  struct seminfo *__buf;
};

#define TEST_ERROR                                                             \
  if (errno)                                                                   \
  {                                                                            \
    fprintf(stderr, "%s:%d: PID=%5d: Error %d (%s)\n", __FILE__, __LINE__,     \
	    getpid(), errno, strerror(errno));                                 \
  }

int sem_set_val(int sem_id, int sem_num, int sem_val);
int sem_reserve(int sem_id, int sem_num);
int sem_release(int sem_id, int sem_num);
void sem_getall(char *my_string, int sem_id);