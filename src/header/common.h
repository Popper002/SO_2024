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
/* __LINE__ is a preprocessor macro that expands to current line number in the source file, as an integer.
     __LINE__ is useful when generating log statements
 */
#endif
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <time.h>
#define ATOM_PATH "./atom"
#define ACTIVATOR_PATH "./active"
#define FUEL_PATH "./fuel"
void fetch_args(char const *argv[]); 
struct config
{
    int N_ATOMI_INIT; 
    int ENERGY_DEMAND;
    int N_ATOM_MAX;
    int MIN_A_ATOMICO;
    int N_NUOVI_ATOMI;
    int  SIM_DURATION;
    int ENERGY_EXPLODE_THRESHOLD;
};
struct config config; 


#endif 
