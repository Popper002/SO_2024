#ifndef ATOM_H
#define ATOM_H
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/cdefs.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/param.h>
#include <stdlib.h>
#include <time.h>

#include "common.h"
#include "ipc.h"

struct atom
{
    pid_t pid; 
    int atomic_number; 
    int atomic_flag;
};


#endif