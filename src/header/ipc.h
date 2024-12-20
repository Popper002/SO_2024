#pragma once
#include <sys/sem.h>
#include <sys/types.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <sys/msg.h> 
#define ALL 0666
#define O_READ  0400
#define O_WRITE 0020
#define KEY_SHM 2111
#define NEW_KEY 0x01
#define MASTER_ACTIVATOR_SEM 0x123
/* */
#define SEM_ID_ATOM 0 
#define SEM_ID_ACTIVATOR 1 
#define SEM_ID_FUEL 2 
#define SEM_ID_INIBITOR 3 
#define TYPE_PROC 4 
struct message
{
    long m_type;
    int data;   
};
struct mes
{
    long m_type;
    char text[10];  
};

typedef struct 
{
    int array[100];
}shm_fuel;

key_t key_sem_m;
key_t key_sem_acti;
