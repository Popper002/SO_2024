#pragma once
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

#ifndef _GNU_SOURCE /* Missing this baby :p */
#define _GNU_SOURCE
#endif 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/param.h>
#include <signal.h>
#include <sys/signal.h>

#define ATOM_PATH "./bin/atom"
#define ACTIVATOR_PATH "./bin/activator"
#define FUEL_PATH "./bin/fuel"
#define INHIBITOR_PATH "./bin/inhibitor"
#define ATOMIC_KEY 0x221
void fetch_args(char const *argv[]); 
int randomize_atom(int atomic_number);
struct config
{
    int N_ATOMI_INIT; 
    int ENERGY_DEMAND;
    int N_ATOM_MAX;
    int MIN_A_ATOMICO;
    int N_NUOVI_ATOMI;
    int SIM_DURATION;
    int ENERGY_EXPLODE_THRESHOLD;
    float STEP;
    int  INHIBITOR;
};
typedef struct 
{
    struct config conf; 
    int num_activation;
    int num_activation_last_sec;
    int num_fission_last_sec;
    int num_fission; 
    int num_energy_produced; 
    int num_energy_produced_last_sec; 
    int num_energy_consumed;
    int num_energy_consumed_last_sec;
    int num_energy_consumed_inebitore; 
    int num_energy_consumed_inebitore_last_sec; 
    /* Forse servono altri parametri --> punto:8 ,.pdf*/


}statistiche; 
/* RIPASSO ENUM: 
* Sono un modo per rappresentare un gruppo di constanti ,di default i valori vanno da 
* 0 a n costanti   */
enum term_reason
{ 
    TIMEOUT  = 0,
    EXPLODE  = 1,
    BLACKOUT = 2,
    MELTDOWN = 3,

};



//TODO Creare una serie di cofigurazioni in modo tale da avere almeno una per ogni term_reason
