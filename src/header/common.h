#ifndef COMMON_H
#define COMMON_H
#ifdef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#ifndef TEST_ERROR
#define TEST_ERROR                                 \
    if (errno)                                     \
    {                                              \
        fprintf(stderr,                            \
                "%s:%d: PID=%5d: Error %d (%s)\n", \
                __FILE__,                          \
                __LINE__,                          \
                getpid(),                          \
                errno,                             \
                strerror(errno));                  \
    }

#endif
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct config
{
    int N_ATOMI_INIT; 
    int ENERGY_DEMAND;
    int N_ATOM_MAX;
    int MIN_A_ATOMICO;
    int N_NUOVI_ATOMI;
    float SIM_DURATION;
    int ENERGY_EXPLODE_THRESHOLD;
};
#endif 
