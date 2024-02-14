#include "my_sem_lib.h"
#include <sys/sem.h>

int sem_set_val(int sem_id, int sem_num, int sem_val)
{
  return semctl(sem_id, sem_num, SETVAL, sem_val);
}

int sem_reserve(int sem_id, int sem_num)
{
  struct sembuf sops;

  sops.sem_num = sem_num;
  sops.sem_op = -1;
  sops.sem_flg = 0;
  return semop(sem_id, &sops, 1);
}

int sem_release(int sem_id, int sem_num)
{
  struct sembuf sops;

  sops.sem_num = sem_num;
  sops.sem_op = 1;
  sops.sem_flg = 0;
  return semop(sem_id, &sops, 1);
}

/**
 * NOTE: this function is a int function in the example, but it has no return
 * value so it gives a warning, I changed it to void for now, so we have to
 * check if it give unexpected results
 */
void sem_getall(char *my_string, int sem_id)
{
  union semun arg;
  unsigned short *sem_vals, i;
  unsigned long num_sem;
  struct semid_ds my_ds;
  char cur_str[10];

  arg.buf = &my_ds;
  semctl(sem_id, 0, IPC_STAT, arg);
  TEST_ERROR;
  num_sem = arg.buf->sem_nsems;

  sem_vals = malloc(sizeof(*sem_vals) * num_sem);
  arg.array = sem_vals;
  semctl(sem_id, 0, GETALL, arg);

  my_string[0] = 0;
  for (i = 0; i < num_sem; i++)
  {
    sprintf(cur_str, "%d ", sem_vals[i]);
    strcat(my_string, cur_str);
  }
}