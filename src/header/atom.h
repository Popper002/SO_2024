#ifndef ATOM_H
#define ATOM_H
#include <unistd.h>
#define ATOMIC_KEY 0x221
struct atom
{
    pid_t pid; 
    int atomic_number; 
};


#endif