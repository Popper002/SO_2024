#include "header/master.h"
#include "header/common.h"
#include "util/hash_table.h"
#include "util/my_sem_lib.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

char **args_atom[100];
char **activator_args[100];
char **activator_args[100];
char const *args_[100];
static int atom_array_pid[100];
static int activator_array_pid[10];
static int fuel_array_pid[100];
int sem_id;
pid_t atom_pid;
pid_t activator_pid;
struct config config;
struct hash_table table;
#ifdef _PRINT_TEST
static void print_para_TEST()
{
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
}
#endif
static int scan_data(FILE *fp)
{
  int value;
  char name_param[500];
  int error = 1;
  printf("Reading data from file...\n");
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
    else
    {
      error = 1;
    }
  }
  printf("Data read from file!\n");
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
  char atomic_number[10];
  sprintf(n_atomi_init, "%d", config.N_ATOMI_INIT);
  sprintf(energy_demand, "%d", config.ENERGY_DEMAND);
  sprintf(n_atom_max, "%d", config.N_ATOM_MAX);
  sprintf(min_a_atomico, "%d", config.MIN_A_ATOMICO);
  sprintf(n_nuovi_atomi, "%d", config.N_NUOVI_ATOMI);
  sprintf(sim_duration, "%d", config.SIM_DURATION);
  sprintf(energy_explode_threshold, "%d", config.ENERGY_EXPLODE_THRESHOLD);
  argv[1] = strdup(n_atomi_init);
  argv[2] = strdup(energy_demand);
  argv[3] = strdup(n_atom_max);
  argv[4] = strdup(min_a_atomico);
  argv[5] = strdup(n_nuovi_atomi);
  argv[6] = strdup(sim_duration);
  argv[7] = strdup(energy_explode_threshold);
  argv[8] = NULL;
}

static int randomize_atom(int atomic_number)
{

  atomic_number = rand() % config.N_ATOM_MAX;
  return atomic_number;
}

pid_t fuel_generator()
{
  pid_t fuel_pid;
  switch (fuel_pid = fork())
  {
  case -1:
    TEST_ERROR;
    exit(EXIT_FAILURE);
  case 0:
    argument_creator((char **)args_atom);
    execve(FUEL_PATH, (char **)args_atom, NULL);
    fprintf(stderr,
	    "%s LINE: %d[MASTER %d  , FUEL_GENERATOR(){PROBLEM IN EXECVP}\n",
	    __func__, __LINE__, getpid());
    exit(EXIT_FAILURE);
    break;

  default:
    if (sem_release(sem_id, 0) == -1)
    {
      fprintf(stderr, "[%s]Error in sem_release %s\n", __FILE__,
	      strerror(errno));
      exit(EXIT_FAILURE);
    }
    return fuel_pid;
    break;
  }
}

pid_t atom_gen(struct config config, struct hash_table table)
{

  int random_a_number = randomize_atom(config.MIN_A_ATOMICO);
  switch (atom_pid = fork())
  {
  case -1:
    TEST_ERROR;
    exit(EXIT_FAILURE);
  case 0:
#ifdef _PRINT_TEST
    printf(" %s %d ,%s\n", __FILE__,getpid(),__func__);
#endif
    argument_creator((char **)args_atom);
    // table.put(&table, atom_pid, random_a_number);
    execvp(ATOM_PATH, (char **)args_atom);
    fprintf(stderr,
	    "%s line: %d[master %d  , ATOM_GENERATOR(){PROBLEM IN EXECVE} \n",
	    __func__, __LINE__, getpid());
    exit(EXIT_FAILURE);
    break;

  default:
    if (sem_release(sem_id, 0) == -1)
    {
      fprintf(stderr, "[%s]Error in sem_release %s\n", __FILE__,
	      strerror(errno));
      exit(EXIT_FAILURE);
    }

    return atom_pid;
    break;
  }
}
pid_t activator(struct config config)
{
 
  switch (activator_pid = fork())
  {
  case -1:
    TEST_ERROR;
    exit(EXIT_FAILURE);
  case 0:
    argument_creator((char **)activator_args);
    execvp(ACTIVATOR_PATH, (char **)activator_args);
    fprintf(stderr, "in: %s line: %d[master %d--> problem in execvp %s}\n",
	    __func__, __LINE__, getpid(), strerror(errno));
    exit(EXIT_FAILURE);
    break;

  default:
    if (sem_release(sem_id, 0))
    {
      fprintf(stderr, "[%s]Error in sem_release %s\n", __FILE__,
	      strerror(errno));
      exit(EXIT_FAILURE);
    }
    return activator_pid;
    break;
  }
}

struct hash_table init_table(struct hash_table table)
{
  table.max = config.N_ATOM_MAX;
  table.number_of_elements = 0;
  table.elements = malloc(table.max * sizeof(struct elem_hash_table *));
  table.put = put;
  table.get = get;
  table.remove = remove_elem;
  table.decrement_usage_count = decrement_usage_count;
  table.garbage_collector = garbage_collect;
  return table;
}
void shutdown()
{
  for (int i = 0; i < config.N_ATOMI_INIT; i++)
  {
    printf("KILLING %d PROC\n", atom_array_pid[i]);
    kill(atom_array_pid[i], SIGKILL);
    kill(activator_array_pid[i],SIGKILL); 
    printf("KILLED\n");
  }
  printf("REMOVING SEM %d\n", sem_id);
  semctl(sem_id, 0, IPC_RMID);
  printf("REMOVED SEM\n");
}
void store_pid_atom()
{
  for (int i = 0; i < config.N_ATOMI_INIT; i++)
  {
    atom_array_pid[i] = atom_gen(config, table);
    /*
#ifdef _PRINT_TEST
    printf("[MASTER %d ] %s , [PID %d ] [POS %d]\n", getpid(), __func__,
	   atom_pid, i);
#endif
*/ 
  }
}
void handle_signal(int signum)
{
  switch (signum)
  {
  case SIGINT:

    break;
  case SIGSTOP:
    break;
  case SIGALRM:
    break;
  default:
    break;
  }
}
#ifdef _PRINT_TEST
void print_all_pid()
{
  printf("-----------------------------------\n");
  for (int i = 0; i < config.N_ATOMI_INIT; i++)
  {
    printf("[MASTER %d ][ATOM] %s , [PID %d ] [POS %d]\n", getpid(), __func__,
	   atom_pid, i);
  }
  for( int  i =0 ; i <config.ENERGY_DEMAND ; i++ )
  { 
    printf("[MASTER %d ][ACTIVATOR] %s ,[PID%d ] [POS %d ]\n",getpid(),__func__ ,activator_pid,i );
  }
  printf("-----------------------------------\n");
}
#endif
int main(int argc, char const *argv[])
{

  init_table(table);
  pid_t atom;
  FILE *fp = fopen("src/config/config1.txt", "r");
  if (fp == NULL)
  {
    fprintf(stderr, "%d\n", errno);
    exit(EXIT_FAILURE);
  }
  sem_id = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);
  if (sem_id == -1)
  {
    fprintf(stderr, "[%s]Error in semget %s\n", __FILE__, strerror(errno));
    exit(EXIT_FAILURE);
  }

  if (sem_set_val(sem_id, 0, 0) == -1)
  {
    fprintf(stderr, "[%s]Error in sem_set_val %s\n", __FILE__, strerror(errno));
  }

  srand(time(NULL));
  printf("MAIN %d\n", getpid());
  scan_data(fp);
#ifdef _PRINT_TEST
  print_para_TEST(config);
#endif
  args_atom[0] = (char **)ATOM_PATH;
  activator_args[0] = (char **)ACTIVATOR_PATH;
  activator_array_pid[0] = activator(config);
/*  
#ifdef _PRINT_TEST
  printf("[MASTER %d ] [%s ] [ACTIVATOR PID %d ]\n", getpid(), __func__,
	 activator_array_pid[0]);
#endif
*/ 
  for (int i = 0; i < config.N_ATOMI_INIT; i++)
  {
#ifdef _PRINT_TEST
    printf("INSIDE FOR LOOP MASTER %d \n", getpid());
#endif
    atom_gen(config, table);
  }
  store_pid_atom();
  #ifdef _PRINT_TEST
    print_all_pid(); 
    #endif
  if (sem_release(sem_id, 0) == -1)
  {
    fprintf(stderr, "[%s]Error in sem_reserve %s\n", __FILE__, strerror(errno));
    exit(EXIT_FAILURE);
  }
  // TODO: qui dovremmo aver finito di creare i processi, a sto punt facciamo
  // sem_release e da qui sotto in poi dovrebbe iniziare la simulazione vera e
  // propria ?

  shutdown();
  // print_hash_table(&table);

  fclose(fp);

  return 0;
}
