#include "header/atom.h"
#include "header/common.h"
#include "header/ipc.h"
#include "util/my_sem_lib.h"
#include <complex.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/param.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#define CONFIG_PATH "src/config/config1.txt"

char **args_atom[100];
char **activator_args[100];
char **fuel_args[100];
char **inhibitor_args[100];

struct statistics *shared_data;
struct atom atom_stat;
// char const *args_[100];
bool is_inhibitor_running = true;
static int shm_id;
static int rcv_id;
// static int fork_atom;
// static int fork_inhibitor;
shm_fuel *rcv_pid;
int *atom_array_pid;
static key_t key_shm;
pid_t atom_pid;
static pid_t inhibitor_pid;
pid_t inhiitor_pid;
pid_t activator_pid;
pid_t fuel_pid;
struct config config;
enum term_reason term_reason;
struct statistics *shared_data;
static int inhibitor_energy_consumed;
int inhibitor_balance;
int total_nuclear_waste;
int num_fission_last_sec;
int num_activation_last_sec;
int energy_produced;
int received_value;
int energy_absorbed;
int energy_produced = 0;

/* #ifdef _PRINT_TEST
static void print_para_TEST()
{

  printf("\t\n----------\n");
  printf("N_ATOMI_INIT: %d\n"
	 "ENERGY_DEMAND :%d\n"
	 "N_ATOM_MAX:%d\n"
	 "MIN_A_ATOMICO :%d\n"
	 "N_NUOVI_ATOMI :%d\n"
	 "SIM_DURATION :%d\n"
	 "ENERGY_EXPLODE_THRESHOLD :%d\n",
	 config.N_ATOMI_INIT, config.ENERGY_DEMAND, config.N_ATOM_MAX,
	 config.MIN_A_ATOMICO, config.N_NUOVI_ATOMI, config.SIM_DURATION,
	 config.ENERGY_EXPLODE_THRESHOLD);

  printf("\t\n----------\n");
}
 #endif */

/*
void remove_ipc() {
    int remove_queue;
    int status;

    / Rimuove gli oggetti IPC solo se i processi sono terminati
    while ((waitpid(inhibitor_pid, &status, WNOHANG) > 0) &&
	   (waitpid(atom_pid, &status, WNOHANG) > 0) &&
	   (waitpid(fuel_pid, &status, WNOHANG) > 0))
    {
	remove_queue = msgget(ATOMIC_KEY, IPC_CREAT); // Ottiene l'ID della coda
da rimuovere TEST_ERROR rcv_id = msgget(STATISTICS_KEY, IPC_CREAT); TEST_ERROR
	fprintf(stdout, "[REMOVE_IPC QUEUE_ID: %d ]\n", remove_queue);
	msgctl(remove_queue, IPC_RMID, NULL);
	msgctl(rcv_id, IPC_RMID, NULL);
	shmctl(shm_id, IPC_RMID, NULL);
	fprintf(stdout, "REMOVED ALL IPC ITEMS\n");
    }
}*/

void remove_ipc()
{
  int remove_queue;
  // int status;
  remove_queue = msgget(ATOMIC_KEY, IPC_CREAT | 0666);
  rcv_id = msgget(STATISTICS_KEY, IPC_CREAT | 0666);

  if (remove_queue < 0)
  {
    fprintf(stderr, "Error in removing ipc object %s %s %d\n", __FILE__,
	    strerror(errno), __LINE__);
    exit(EXIT_FAILURE);
  }

  if (msgctl(remove_queue, IPC_RMID, NULL) == -1)
  {
    fprintf(stderr, "Error in removing ipc object %s %s %d\n", __FILE__,
	    strerror(errno), __LINE__);
    exit(EXIT_FAILURE);
  }

  if (rcv_id < 0)
  {
    fprintf(stderr, "Error in removing ipc object %s %s %d\n", __FILE__,
	    strerror(errno), __LINE__);
    exit(EXIT_FAILURE);
  }

  if (msgctl(rcv_id, IPC_RMID, NULL) == -1)
  {
    fprintf(stderr, "Error in removing ipc object %s %s %d\n", __FILE__,
	    strerror(errno), __LINE__);
    exit(EXIT_FAILURE);
  }

  if (shmctl(shm_id, IPC_RMID, NULL) == -1)
  {
    fprintf(stderr, "Error in removing ipc object %s %s %d\n", __FILE__,
	    strerror(errno), __LINE__);
    exit(EXIT_FAILURE);
  }

  fprintf(stdout, "Removed all ipc object\n");

  if (msgctl(remove_queue, IPC_RMID, NULL) == -1)
  {
    fprintf(stderr, "Error in removing ipc object  %s %s %d\n", __FILE__,
	    strerror(errno), __LINE__);
    exit(EXIT_FAILURE);
  }
  if (msgctl(rcv_id, IPC_RMID, NULL) == -1)
  {
    fprintf(stderr, "Error in removing ipc object %s %s %d\n", __FILE__,
	    strerror(errno), __LINE__);
    exit(EXIT_FAILURE);
  }

  if (shmctl(shm_id, IPC_RMID, NULL) == -1)
  {
    fprintf(stderr, "Error in removing ipc object %s %s %d\n", __FILE__,
	    strerror(errno), __LINE__);
    exit(EXIT_FAILURE);
  }
}

void kill_them_all()
{
  killpg(*atom_array_pid, SIGKILL);
  killpg(activator_pid, SIGKILL);
  killpg(inhibitor_pid, SIGKILL);
  killpg(fuel_pid, SIGKILL);
}

int why_term(enum term_reason term_reason)
{
  switch (term_reason)
  {
  case EXPLODE:
    kill_them_all();
    remove_ipc();
    write(STDOUT_FILENO, "TOO MUCH ENERGY REALEASED\n",27);
    exit(EXIT_FAILURE);
    break;
  case BLACKOUT:
    write(STDOUT_FILENO, "BLACKOUT- NOT ENOUGH ENERGY\n", 29);
    kill_them_all();
    remove_ipc();
    exit(EXIT_FAILURE);
    break;
  case MELTDOWN:
    write(STDOUT_FILENO, "MELTDOWN - FORK-ERROR \n", 24);
    remove_ipc();
    kill_them_all();
    exit(EXIT_FAILURE);
    break;
  default:
    return term_reason;
    break;
  }
}

static int scan_data(char *file_path)
{
  int value;
  char name_param[500];
  int error = 1;
  FILE *fp = fopen(file_path, "r");
  if (fp == NULL)
  {
    fprintf(stderr, "%d\n", errno);
    exit(EXIT_FAILURE);
  }
  /* #ifdef _PRINT_TEST
  printf("Reading data from file...\n");
   #endif */
  while (fscanf(fp, "%s %d", name_param, &value) != EOF)
  {
    if (strcmp(name_param, "N_ATOMI_INIT") == 0)
    {
      config.N_ATOMI_INIT = value;
      error = 0;
    }
    else if (strcmp(name_param, "ENERGY_DEMAND") == 0)
    {
      config.ENERGY_DEMAND = value;
      error = 0;
    }
    else if (strcmp(name_param, "N_ATOM_MAX") == 0)
    {
      config.N_ATOM_MAX = value;
      error = 0;
    }
    else if (strcmp(name_param, "MIN_A_ATOMICO") == 0)
    {
      config.MIN_A_ATOMICO = value;
      error = 0;
    }
    else if (strcmp(name_param, "N_NUOVI_ATOMI") == 0)
    {
      config.N_NUOVI_ATOMI = value;
      error = 0;
    }
    else if (strcmp(name_param, "SIM_DURATION") == 0)
    {
      config.SIM_DURATION = value;
      error = 0;
    }
    else if (strcmp(name_param, "ENERGY_EXPLODE_THRESHOLD") == 0)
    {
      config.ENERGY_EXPLODE_THRESHOLD = value;
      error = 0;
    }
    else if (strcmp(name_param, "INHIBITOR") == 0)
    {
      config.INHIBITOR = value;
      errno = 0;
    }
    else if (strcmp(name_param, "STEP") == 0)
    {
      config.STEP = value;
      errno = 0;
    }
    else
    {
      error = 1;
    }
  }

  fclose(fp);
  /* #ifdef _PRINT_TEST
  printf("Data read from file!\n");
  if (config.INHIBITOR == 1)
  {
    printf("Inhibitor is going to be used in this simulation\n");
  }
   #endif */

  return error;
}
static void argument_creator(char *argv[])
{
  char n_atomi_init[10];
  char energy_demand[10];
  char n_atom_max[10];
  char min_a_atomico[10];
  char n_nuovi_atomi[10];
  char sim_duration[10];
  char energy_explode_threshold[10];
  char master_pid[10];
  // char atomic_number[10];
  sprintf(n_atomi_init, "%d", config.N_ATOMI_INIT);
  sprintf(energy_demand, "%d", config.ENERGY_DEMAND);
  sprintf(n_atom_max, "%d", config.N_ATOM_MAX);
  sprintf(min_a_atomico, "%d", config.MIN_A_ATOMICO);
  sprintf(n_nuovi_atomi, "%d", config.N_NUOVI_ATOMI);
  sprintf(sim_duration, "%d", config.SIM_DURATION);
  sprintf(energy_explode_threshold, "%d", config.ENERGY_EXPLODE_THRESHOLD);
  sprintf(master_pid,"%d",getpid());
  argv[1] = strdup(n_atomi_init);
  argv[2] = strdup(energy_demand);
  argv[3] = strdup(n_atom_max);
  argv[4] = strdup(min_a_atomico);
  argv[5] = strdup(n_nuovi_atomi);
  argv[6] = strdup(sim_duration);
  argv[7] = strdup(energy_explode_threshold);
  argv[8] = strdup(master_pid);
  argv[9] = NULL;
}
static void fuel_argument_ipc(char *argv[])
{

  char n_atomi_init[10];
  char energy_demand[10];
  char n_atom_max[10];
  char min_a_atomico[10];
  char n_nuovi_atomi[10];
  char sim_duration[10];
  char energy_explode_threshold[10];
  char step[10];
  // char atomic_number[10];
  sprintf(n_atomi_init, "%d", config.N_ATOMI_INIT);
  sprintf(energy_demand, "%d", config.ENERGY_DEMAND);
  sprintf(n_atom_max, "%d", config.N_ATOM_MAX);
  sprintf(min_a_atomico, "%d", config.MIN_A_ATOMICO);
  sprintf(n_nuovi_atomi, "%d", config.N_NUOVI_ATOMI);
  sprintf(sim_duration, "%d", config.SIM_DURATION);
  sprintf(energy_explode_threshold, "%d", config.ENERGY_EXPLODE_THRESHOLD);
  sprintf(step, "%ld", config.STEP);
  /* ipc */
  char ipc_shm_id_[10];
  char ipc_shm_key_[10];
  sprintf(ipc_shm_id_, "%d", shm_id);
  sprintf(ipc_shm_key_, "%d", key_shm);
  argv[1] = strdup(n_atomi_init);
  argv[2] = strdup(energy_demand);
  argv[3] = strdup(n_atom_max);
  argv[4] = strdup(min_a_atomico);
  argv[5] = strdup(n_nuovi_atomi);
  argv[6] = strdup(sim_duration);
  argv[7] = strdup(energy_explode_threshold);
  /* */
  char shm_id_str[10];
  char key_shm_str[10];
  sprintf(shm_id_str, "%d", shm_id);
  sprintf(key_shm_str, "%d", key_shm);
  argv[8] = strdup(shm_id_str);
  argv[9] = strdup(key_shm_str);

  argv[10] = strdup(step);
  argv[11] = NULL;
}

pid_t inhibitor(void)
{

  switch (inhiitor_pid = fork())
  {
  case -1:
    why_term(MELTDOWN);
    break;
  case 0:
    argument_creator((char **)inhibitor_args);
    printf("\tStarting inhbitior HERE\n");
    execvp(INHIBITOR_PATH, (char **)inhibitor_args);

    fprintf(stderr, "%s line: %d [master %s Problem in execvp with pid %d \n",
	    __func__, __LINE__, __FILE__, getpid());
    exit(EXIT_FAILURE);
    break;

  default:
    return inhiitor_pid;
    break;
  }
  return inhibitor_pid;
}
pid_t fuel_generator(void)
{
  pid_t fuel_pid;
  switch (fuel_pid = fork())
  {
  case -1:
    why_term(MELTDOWN);
    break;
  case 0:
    /* #ifdef _PRINT_TEST
	printf("fuel case 0\n");
     #endif */
    fuel_argument_ipc((char **)fuel_args);
    execvp(FUEL_PATH, (char *const *)fuel_args);
    fprintf(
	stderr,
	"%s LINE: %d[MASTER %d  , FUEL_GENERATOR(){PROBLEM IN EXECVP}, %s\n",
	__func__, __LINE__, getpid(), strerror(errno));
    exit(EXIT_FAILURE);
    break;

  default:
    return fuel_pid;
    break;
  }
  return fuel_pid;
}

pid_t atom_gen(void)
{
  int status;
  switch (atom_pid = fork())
  {
  case -1:
    why_term(MELTDOWN);
    break;
  case 0:

    argument_creator((char **)args_atom);
    execvp(ATOM_PATH, (char **)args_atom);

    fprintf(stderr, "%s line: %d[master %s Problem in execvp with pid %d \n",
	    __func__, __LINE__, __FILE__, getpid());
    exit(EXIT_FAILURE);
    break;

  default:

    waitpid(atom_pid, &status, WNOHANG);
    return atom_pid;
    break;
  }
  return atom_pid;
}
pid_t activator(void)
{

  switch (activator_pid = fork())
  {
  case -1:
    why_term(MELTDOWN);
    break;
  case 0:
    /* #ifdef _PRINT_TEST
	printf("activator case 0\n");
     #endif */

    argument_creator((char **)activator_args);
    execvp(ACTIVATOR_PATH, (char **)activator_args);
    fprintf(stderr, "in: %s line: %d[master %d--> problem in execvp %s}\n",
	    __func__, __LINE__, getpid(), strerror(errno));
    exit(EXIT_FAILURE);
    break;

  default:
    return activator_pid;
    break;
  }
  return activator_pid;
}

void store_pid_atom()
{
  atom_array_pid = (int *)malloc(config.N_ATOMI_INIT * sizeof(int));
  if (atom_array_pid == NULL)
  {
    fprintf(stdout, "malloc error :ERR:%s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < config.N_ATOMI_INIT; i++)
  {
    atom_array_pid[i] = atom_gen();
    /* #ifdef _PRINT_TEST
	printf("[MASTER %d ] %s , [PID %d ] [POS %d]\n", getpid(), __func__,
	       atom_array_pid[i], i);
     #endif */
  }

  /* #ifdef _PRINT_TEST
   fprintf(stdout, "Child process %d created and suspended.\n", atom_pid);
    #endif */
  free(atom_array_pid);
}

/**
  sizeof(int *) / sizeof(int) gives a warning with -Wall, so we had to use new
  way to compute the size of atom_array_pid
  https://arjunsreedharan.org/post/69303442896/how-to-find-the-size-of-an-array-in-c-without
  here there is an explanation on why this work
*/
void handle_signal(int signum)
{
  switch (signum)
  {
  case SIGINT:
    write(STDOUT_FILENO, "SIGINT_HANDLE\n", 15);
    int length = (&atom_array_pid)[1] - atom_array_pid;
    for (int pid = 0; pid < length; pid++)
    {
      kill(atom_array_pid[pid], SIGINT);
    }
    break;
  case SIGUSR1:
    printf("signal sigusr1 received");
    why_term(MELTDOWN);
    break;
  case SIGALRM:

    write(STDOUT_FILENO, "\n\t-----------------------------------\n", 39);
    write(STDOUT_FILENO, "\t\tALARM : IT'S TIME TO STOP\n", 29);
    write(STDOUT_FILENO, "\n\t-----------------------------------\n", 39);

    killpg(*atom_array_pid, SIGKILL);
    killpg(activator_pid, SIGKILL);
    killpg(inhibitor_pid, SIGKILL);
    killpg(fuel_pid, SIGKILL);
    write(STDOUT_FILENO, "PRIMA DI REMOVE STO CAZZO DI IPC\n", 34);
    remove_ipc();
    // total_print();

    write(STDOUT_FILENO, "TEARM_REASON < TIMEOUT >\n", 26);
    exit(EXIT_SUCCESS);

    break;
  case SIGCHLD:
    wait(NULL);
    break;
  default:
    break;
  }
}

void start_atom()
{
  printf("\033[1;32m starting atom as last process \033[0m\n");

  for (int i = 0; i < config.N_ATOMI_INIT; i++)
  {
    kill(atom_array_pid[i], SIGCONT);
  }
  /*#ifdef __PRINT_TEST
      printf("\n\tSTART ATOM %d\n", atom_array_pid[i]);
  #endif */
}

// use sigint to start and stop the inhibitor
void inhibitor_handle(int signum)
{
  if (signum == SIGINT)
  {
    if (is_inhibitor_running)
    {
      kill(inhibitor_pid, SIGSTOP);
      write(STDOUT_FILENO, "STOPPED INHIBITOR\n", 19);
      is_inhibitor_running = false;
    }
    else
    {
      kill(inhibitor_pid, SIGCONT);
      write(STDOUT_FILENO, "STARTED INHIBITOR\n", 19);
      is_inhibitor_running = true;
    }
  }

  /*
  switch (signum)
  {
    case SIGUSR1:
      fprintf(stdout, "START INHIBITOR\n");
      kill(inhibitor_pid, SIGCONT);
      break;

    case SIGUSR2:
      fprintf(stdout, "STOPPING INHIBITOR\n");
      kill(inhibitor_pid, SIGSTOP);
      break;

    default:
      fprintf(stderr, "Invalid signal received\n");
      break;
  }
   */
}

/**
@brief: string formatting in C
https://stackoverflow.com/questions/25626851/align-text-to-right-in-c
*/
void total_print(void)
{
  static struct message rcv_stats;
  rcv_id = msgget(STATISTICS_KEY, IPC_CREAT | 0666);
  if (rcv_id < 0)
  {
    fprintf(stderr, "[MASTER] ERROR MSGGET\n");
    exit(EXIT_FAILURE);
  }

  msgrcv(rcv_id, &rcv_stats, sizeof(rcv_stats), 1, IPC_NOWAIT);
  num_activation_last_sec += rcv_stats.data;
  msgrcv(rcv_id, &rcv_stats, sizeof(rcv_stats), 2, IPC_NOWAIT);
  energy_produced += rcv_stats.data;

  msgrcv(rcv_id, &rcv_stats, sizeof(rcv_stats), 3, IPC_NOWAIT);
  total_nuclear_waste = rcv_stats.data;

  msgrcv(rcv_id, &rcv_stats, sizeof(rcv_stats), 5, IPC_NOWAIT);
  energy_absorbed += rcv_stats.data;
  msgrcv(rcv_id, &rcv_stats, sizeof(rcv_stats), 6, IPC_NOWAIT);
  statistics_data.num_energy_consumed_inhibitor_last_sec += rcv_stats.data;
  msgrcv(rcv_id, &rcv_stats, sizeof(rcv_stats), 7, IPC_NOWAIT);
  inhibitor_balance += rcv_stats.data;
  msgrcv(rcv_id, &rcv_stats, sizeof(rcv_stats), 8, IPC_NOWAIT);
  num_fission_last_sec += rcv_stats.data;

  statistics_data.num_energy_consumed_last_sec =
      energy_produced - config.ENERGY_DEMAND;
  if (energy_produced < 0)
  {
    why_term(BLACKOUT);
  }

  if (energy_produced > config.ENERGY_EXPLODE_THRESHOLD)
  {
    why_term(EXPLODE);
  }
  printf("\n|===========================|\n");
  printf("| %-20s %d\n", "ACTIVATION_VALUE", num_activation_last_sec);
  printf("| %-20s %d\n", "WASTE_VALUE", total_nuclear_waste);
  if (config.INHIBITOR == 1)
  {
    printf("| %-20s %d\n", "INHIBITOR BALANCE", inhibitor_balance);
    printf("| %-20s %d\n", "ENERGY ABSORBED IN INHIBITOR",
	   inhibitor_energy_consumed);
  }
  printf("| %-20s %d\n", "ENERGY CONSUMED",
	 statistics_data.num_energy_consumed_last_sec);
  printf("| %-20s %d\n", "ENERGY PRODUCED", energy_produced);
  printf("| %-20s %d\n", "FISSION_VALUE", num_fission_last_sec);
  printf("|===========================|\n\n");

  sleep(1);
}

void final_print(struct statistics final_print)
{
  final_print.total_num_activation = final_print.num_activation_last_sec;
  final_print.total_nuclear_waste = final_print.total_nuclear_waste_last_sec;
  final_print.total_num_fission = final_print.num_fission_last_sec;
  final_print.total_num_energy_consumed =
      final_print.total_num_energy_produced_last_sec;
  final_print.energy_absorbed = final_print.energy_absorbed_last_sec;
  final_print.total_num_energy_consumed = final_print.energy_absorbed_last_sec;
  final_print.inhibitor_balancing = final_print.inhibitor_balancing_last_sec;
}

void print_config(void)
{
  printf("\n\n\n");
  printf("\t-----------------------------------\n");
  printf("\t\tCONFIGURATION\n");
  printf("\t-----------------------------------\n");
  printf("\tN_ATOMI_INIT: %d\n", config.N_ATOMI_INIT);
  printf("\tENERGY_DEMAND: %d\n", config.ENERGY_DEMAND);
  printf("\tN_ATOM_MAX: %d\n", config.N_ATOM_MAX);
  printf("\tMIN_A_ATOMICO: %d\n", config.MIN_A_ATOMICO);
  printf("\tN_NUOVI_ATOMI: %d\n", config.N_NUOVI_ATOMI);
  printf("\tSIM_DURATION: %d\n", config.SIM_DURATION);
  printf("\tENERGY_EXPLODE_THRESHOLD: %d\n", config.ENERGY_EXPLODE_THRESHOLD);
  printf("\tINHIBITOR: %d\n", config.INHIBITOR);
  printf("\tSTEP: %ld\n", config.STEP);
  printf("\t-----------------------------------\n");
}

void logo(void)
{
  printf("\t-----------------------------------\n");

  printf("\
           _______ ____  __  __ _____ _____    _____ ______ _   _ ______ _____         \n\
     /\\|__   __/ __ \\|  \\/  |_   _/ ____|  / ____|  ____| \\ | |  ____|  __ \\     /\\|__   __/ __ \\|  __ \\    \n\
    /  \\  | | | |  | | \\  / | | || |      | |  __| |__  |  \\| | |__  | |__) |   /  \\  | | | |  | | |__) |   \n\
   / /\\ \\ | | | |  | | |\\/| | | || |      | | |_ |  __| | . ` |  __| |  _  /   / /\\ \\ | | | |  | |  _  /    \n\
  / ____ \\| | | |__| | |  | |_| || |____  | |__| | |____| |\\  | |____| | \\ \\  / ____ \\| | | |__| | | \\ \\   \n\
 /_/    \\_\\_|  \\____/|_|  |_|_____|\\_____|  \\_____|______|_|_\\_|______|_|  \\_\\/_/    \\_\\_|_|\\____/|_|  \\_\\ \n");

  printf("\t-------------------------------------------\n");

  printf(" By Riccardo Oro & Francesco Mauro \n");
  printf("\n\n\n");

  printf("              _             _         _                 _       _   "
	 "_              \n"
	 "     | |           | |       (_)               | |     | | (_)       "
	 "      \n"
	 "  ___| |_ __ _ _ __| |_   ___ _ _ __ ___  _   _| | __ _| |_ _  ___  "
	 "_ __   \n"
	 " / __| __/ _` | '__| __| / __| | '_ ` _ \\| | | | |/ _` | __| |/ _ "
	 "\\| '_ \\  \n"
	 " \\__ \\ || (_| | |  | |_  \\__ \\ | | | | | | |_| | | (_| | |_| | "
	 "(_) | | | | \n"
	 " |___/\\__\\__,_|_|   \\__| |___/_|_| |_| "
	 "|_|\\__,_|_|\\__,_|\\__|_|\\___/|_| |_| \n"
	 "                                                                     "
	 "      \n"
	 "                                                                     "
	 "      \n");

  print_config();
}
int main(void)
{
  int start;
  key_shm = KEY_SHM; // ftok("header/common.h",'s');
  // void *rcv_ptr;
  // char command[2];
  /* #ifdef _PRINT_TEST
    printf("KEY IS %d \n", key_shm);
   #endif */

  if (key_shm < 0)
  {
    fprintf(stderr, "PROBLEM KEY\n");
  }
  shm_id = shmget(key_shm, sizeof(config.N_ATOMI_INIT), IPC_CREAT | 0666);

  srand(time(NULL));
  signal(SIGALRM, handle_signal);
  signal(SIGCHLD, handle_signal);
  signal(SIGUSR1,handle_signal);

  /*
  signal(SIGUSR1, inhibitor_handle);
  signal(SIGUSR2, inhibitor_handle);
   */

  char file_path[100];
  printf("Insert the configuration path: <src/config/file_name.txt> \n");
  scanf("%s", file_path);
  if(access(file_path, F_OK) == 0){
    scan_data(file_path);
  } else{ 
    printf("wrong file path \n");
    exit(EXIT_FAILURE);
  }

  /* #ifdef _PRINT_TEST
   // print_para_TEST(config);
   #endif */

  args_atom[0] = (char **)ATOM_PATH;
  activator_args[0] = (char **)ACTIVATOR_PATH;
  fuel_args[0] = (char **)FUEL_PATH;
  inhibitor_args[0] = (char **)INHIBITOR_PATH;

  activator_pid = activator();
  /* #ifdef _PRINT_TEST
    printf("[MASTER %d ] [%s ] [ACTIVATOR PID %d ]\n", getpid(), __func__,
	   activator_pid);
   #endif */

  fuel_pid = fuel_generator();
  if (config.INHIBITOR == 1)
  {
    inhibitor_pid = inhibitor();
    printf("inhbitor pid is: %d", inhibitor_pid);
    printf("\tInhibitor started\n");
    signal(SIGINT, inhibitor_handle);
    signal(SIGUSR2, inhibitor_handle);
  }
  store_pid_atom();
  rcv_pid = (shm_fuel *)shmat(shm_id, NULL, 0);

  /* #ifdef _PRINT_TEST
    for (int i = 0; i < config.N_NUOVI_ATOMI; i++)
    {

      printf("[%s]Il valore memorizzato è %d\n", __FILE__, rcv_pid->array[i]);
    }
    fprintf(stdout, "atoms generated and stopped\n");
   #endif */

  // shutdown();
  /* #ifdef _PRINT_TEST
  printf("\n\t\t\tMaster process didn't kill himself :)\n\n");
  printf("\n\t-----------------------------------\n");
  printf("\t\tEverything is ready to start the simulation\n");
  printf("\n\t-----------------------------------\n");
  printf("\n\t\t\tMaster process didn't kill himself :)\n\n");
   #endif */
  logo();
  for (start = 5; start > 0; start--)
  {
    printf("\rStarting the simulation in %d...", start);
    fflush(stdout);
    sleep(1);
  }
  fprintf(stdout, "\nMaster: [PID %d] is starting the simulation\n", getpid());
  alarm(config.SIM_DURATION);

  start_atom();

  while (1)
  {
    /*
    while(config.SIM_DURATION--){
    printf("TIME REMANING FOR SIMULATION %d \n", config.SIM_DURATION);
*/
    total_print();
    //     TODO: call a function that displays statistic
    /*
   if (config.INHIBITOR == 1)
{
  fprintf(stdout, "Comando <s: start / t:stop >");


  // Legge il comando dall'utente
  if (fgets(command, sizeof(command), stdin) == NULL)
  {
    break;
  }

  // Rimuove il carattere newline finale se presente
  command[strcspn(command, "\n")] = '\0';

  // Esegue il comando corrispondente
  switch (command[0])
  {
    case 's':
      if (inhibitor == 0 || inhibitor == NULL)
      {
	inhibitor_pid = inhibitor();
      }
      else
      {
	kill(inhibitor_pid, SIGUSR1);
      }
      break;

    case 't':
      if (inhibitor == 0 || inhibitor == NULL)
      {
	inhibitor_pid = inhibitor();
      }
      else
      {
	kill(inhibitor_pid, SIGUSR2);
      }
      break;

    default:
      fprintf(stderr, "Comando non valido\n");
      break;
  }
     */
  }

  // detach_shared_memory();
  return 0;
}