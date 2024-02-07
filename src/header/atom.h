#ifndef ATOM_H
#define ATOM_H
#include <unistd.h>

struct atom
{
    pid_t pid; 
    int atomic_number; 
    int atomic_flag;
};


#endif