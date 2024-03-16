#pragma once
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
  double STEP; //da castare
  int INHIBITOR;
};


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
