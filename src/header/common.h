#pragma once


#ifndef _GNU_SOURCE /* Missing this baby :p */
#define _GNU_SOURCE
 #endif 
#include <errno.h>
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <sys/signal.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

#include "ipc.h"
#ifndef TEST_ERROR

#define TEST_ERROR                                                             \
  if (errno)                                                                   \
  {                                                                            \
    fprintf(stderr, "%s:%d: PID=%5d: Error %d (%s)\n", __FILE__, __LINE__,     \
	    getpid(), errno, strerror(errno));                                 \
  }
/* __LINE__ is a preprocessor macro that expands to current line number in the
   source file, as an integer.
     __LINE__ is useful when generating log statements
 */
 #endif 
#define ATOM_PATH "./bin/atom"
#define ACTIVATOR_PATH "./bin/activator"
#define PSU_PATH "./bin/psu"
#define INHIBITOR_PATH "./bin/inhibitor"
#define ATOMIC_KEY 0x221
#define STATISTICS_KEY 0x11
#define ENERGY_ABSORBED_KEY 0x22

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
  time_t STEP;
  int INHIBITOR;
};
struct statistics
    {
      int total_num_activation;
      int num_activation_last_sec;
      int num_fission_last_sec;
      int total_num_fission;
      int energy_produced_value;
      int total_num_energy_produced_last_sec;
      int total_num_energy_consumed;
      int num_energy_consumed_last_sec;
      int num_energy_consumed_inhibitor;
      int num_energy_consumed_inhibitor_last_sec;
      int total_nuclear_waste;
      int total_nuclear_waste_last_sec;
      int energy_absorbed;
      int energy_absorbed_last_sec;
      int total_inhibitor_balancing;
      int inhibitor_balancing_last_sec;
      int total_energy_absorbed;
      int total_energy_produced;
    }statistics_data;



/* RIPASSO ENUM:
 * Sono un modo per rappresentare un gruppo di constanti ,di default i valori
 * vanno da 0 a n costanti   */
enum term_reason
{
  TIMEOUT = 0,
  EXPLODE = 1,
  BLACKOUT = 2,
  MELTDOWN = 3,

};


// TODO Creare una serie di cofigurazioni in modo tale da avere almeno una per
// ogni term_reason
