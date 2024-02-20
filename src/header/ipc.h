#ifndef IPC_H_SO
#define IPC_H_SO
#include <sys/sem.h>
#include <sys/types.h>
#include <semaphore.h>
#include <sys/shm.h>
#define ALL 0666
#define O_READ  0400
#define O_WRITE 0020
#define KEY_SHM 2111

struct message
{
    long m_type;
    char text[10];
};

key_t key_sem_m;
key_t key_sem_acti;
#endif
