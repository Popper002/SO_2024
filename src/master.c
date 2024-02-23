#include "header/master.h"
#include "header/common.h"
#include "header/ipc.h"
#include "util/hash_table.h"
#include "util/my_sem_lib.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define CONFIG_PATH "src/config/config1.txt"

char **args_atom[100];
char **activator_args[100];
char **fuel_args[100];
char **inebitore_args[100];
// char const *args_[100];
static int activator_array_pid[10];
static int fuel_array_pid[100];
static int sem_id;
static int shm_id;
static key_t key_shm;
pid_t atom_pid;
pid_t inhiitor_pid;
pid_t activator_pid;
struct config config;
struct hash_table table;
// static int atom_array_pid[100];
pid_t atom_array_pid[100];

#ifdef _PRINT_TEST
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
#endif
static int scan_data()
{
  int value;
  char name_param[500];
  int error = 1;
  FILE *fp = fopen(CONFIG_PATH, "r");
  if (fp == NULL)
  {
    fprintf(stderr, "%d\n", errno);
    exit(EXIT_FAILURE);
  }
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

  fclose(fp);
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
static void fuel_argument_ipc(char *argv[])
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
  argv[10] = NULL;
}
int randomize_atom(int atomic_number)
{

  atomic_number = rand() % config.N_ATOM_MAX;
  return atomic_number;
}
pid_t inhibitor()
{
  switch (inhiitor_pid = fork())
  {
  case -1:
    TEST_ERROR;
    exit(EXIT_FAILURE);
  case 0:
    argument_creator((char **)inebitore_args);
    execvp(INEBITORE_PATH, (char **)inebitore_args);

    fprintf(stderr, "%s line: %d[master %s Problem in execvp with pid %d \n",
	    __func__, __LINE__, __FILE__, getpid());
    exit(EXIT_FAILURE);
    break;

  default:
  printf("Inebitore case default\n");
    return inhiitor_pid;
    break;
 }
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
  printf("fuel case 0\n");

    printf("fuel generator pid %d\n", getpid());
    fuel_argument_ipc((char **)fuel_args);
    printf("got argument ipc\n");
    execvp(FUEL_PATH, (char *const *)fuel_args);
    fprintf(
	stderr,
	"%s LINE: %d[MASTER %d  , FUEL_GENERATOR(){PROBLEM IN EXECVP}, %s\n",
	__func__, __LINE__, getpid(), strerror(errno));
    exit(EXIT_FAILURE);
    break;

  default:

printf("fuel case default\n");
    return fuel_pid;
    break;
  }
}

pid_t atom_gen(struct config config)
{

  int random_a_number = randomize_atom(config.MIN_A_ATOMICO);
  switch (atom_pid = fork())
  {
  case -1:
    TEST_ERROR;
    exit(EXIT_FAILURE);
  case 0:
  printf("atom case 0\n");
    argument_creator((char **)args_atom);
    execvp(ATOM_PATH, (char **)args_atom);

    fprintf(stderr, "%s line: %d[master %s Problem in execvp with pid %d \n",
	    __func__, __LINE__, __FILE__, getpid());
    exit(EXIT_FAILURE);
    break;

  default:
  printf("atom case default\n");
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
  printf("activator case 0\n");

    argument_creator((char **)activator_args);
    execvp(ACTIVATOR_PATH, (char **)activator_args);
    printf("activator executed\n");
    fprintf(stderr, "in: %s line: %d[master %d--> problem in execvp %s}\n",
	    __func__, __LINE__, getpid(), strerror(errno));
    exit(EXIT_FAILURE);
    break;

  default:
    printf("activator case default\n");
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
  printf("Removed sem %d\n", sem_id);
  semctl(sem_id, 0, IPC_RMID);
  /*
  for (int i = 0; i < config.N_ATOMI_INIT; i++)
  {
    if (atom_array_pid[i] != getpid()) {
      kill(atom_array_pid[i], SIGKILL);
    }
    if (activator_array_pid[i] != getpid()) {
      kill(activator_array_pid[i], SIGKILL);
    }
    printf("Killed process with pid %d\n", atom_array_pid[i]);
    printf("Killed process with pid %d\n", activator_array_pid[i]);
  }
  */
}

void store_pid_atom()
{
  for (int i = 0; i < config.N_ATOMI_INIT; i++)
  {
    atom_array_pid[i] = atom_gen(config);
#ifdef _PRINT_TEST
    printf("[MASTER %d ] %s , [PID %d ] [POS %d]\n", getpid(), __func__,
	   atom_array_pid[i], i);
#endif
  }
}
void handle_signal(int signum)
{
  switch (signum)
  {
  case SIGINT:
    write(STDOUT_FILENO, "SIGINT_HANDLE\n", 15);
    size_t length = sizeof(atom_array_pid) / sizeof(atom_array_pid[0]);
    for(size_t pid = 0; pid < length; pid++)
    {
      kill(atom_array_pid[pid], SIGINT);
    }
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
	   atom_array_pid[i], i);
  }
  for (int i = 0; i < config.ENERGY_DEMAND; i++)
  {
    printf("[MASTER %d ][ACTIVATOR] %s ,[PID%d ] [POS %d ]\n", getpid(),
	   __func__, activator_array_pid[i], i);
  }
  printf("-----------------------------------\n");
}
#endif
void ipc_init()
{

  // if (sem_set_val(sem_id, 0, config.N_ATOMI_INIT) == -1)
  //{
  // fprintf(stderr, "[%s]Error %d in sem_set_val %s\n", __FILE__, errno,
  //   strerror(errno));
  //}
}

int main(int argc, char const *argv[])
{
  shm_fuel *rcv_pid; 
  init_table(table);
  pid_t atom;

  key_shm = KEY_SHM; // ftok("header/common.h",'s');
#ifdef _PRINT_TEST
  printf("KEY IS %d \n", key_shm);
#endif
  if (key_shm < 0)
  {
    fprintf(stderr, "PROBLEM KEY\n");
  }
  shm_id = shmget(key_shm, sizeof(config.N_ATOMI_INIT)*sizeof(pid_t),
		  IPC_CREAT | 0666);
  printf("SHM ID %d\n ", shm_id);
  sem_id = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);
  if (sem_id == -1)
  {
    fprintf(stderr, "[%s]Error in semget %s\n", __FILE__, strerror(errno));
    exit(EXIT_FAILURE);
  }

  srand(time(NULL));
  printf("-> Main %d <-\n", getpid());
  scan_data();
// ipc_init();
#ifdef _PRINT_TEST
  printf("");
#endif
#ifdef _PRINT_TEST
  print_para_TEST(config);
#endif
  args_atom[0] = (char **)ATOM_PATH;
  activator_args[0] = (char **)ACTIVATOR_PATH;
  activator_array_pid[0] = activator(config);
  fuel_args[0] = (char **)FUEL_PATH;
  inebitore_args[0]=(char **)INEBITORE_PATH;

  /*
  #ifdef _PRINT_TEST
    printf("[MASTER %d ] [%s ] [ACTIVATOR PID %d ]\n", getpid(), __func__,
	   activator_array_pid[0]);
  #endif
  */
  pid_t activator_pid = activator(config);
  printf("activator pid %d\n", activator_pid);
  kill(activator_pid, SIGSTOP);
  printf("activator generated and stopped\n");
  pid_t fuel_pid = fuel_generator();
  kill(fuel_pid, SIGSTOP);

  pid_t inhibitor_pid = inhibitor();
  kill(inhibitor_pid, SIGSTOP);
  printf("fuel generated and stopped\n");
  for (int i = 0; i < config.N_ATOMI_INIT; i++)
  {
    pid_t atom_pid = atom_gen(config);
    kill(atom_pid, SIGSTOP);
  }
  printf("atoms generated and stopped\n");
  // //store_pid_atom();
  /*
#ifdef _PRINT_TEST
  //print_all_pid();
#endif
*/
  // TODO: qui dovremmo aver finito di creare i processi, a sto punto facciamo
  // sem_release e da qui sotto in poi dovrebbe iniziare la simulazione vera e
  // propria ?
  // shutdown(); // FIXME: master process killhimself
  printf("\n\t\t\tMaster process didn't kill himself :)\n\n");
  printf("\n\t-----------------------------------\n");
  printf("\t\tEverything is ready to start the simulation\n");
  printf("\n\t-----------------------------------\n");
  rcv_pid = (shm_fuel*) shmat(shm_id , NULL , 0 );
    for( int i =0 ; i < config.N_NUOVI_ATOMI ;i++) 
    {
    printf("[%s]Il valore memorizzato è %d\n", __FILE__,rcv_pid->array[i]);
    }
  shmdt(rcv_pid);
  for (int i = 0; i < config.N_ATOMI_INIT; i++)
  {
    kill(atom_array_pid[i], SIGCONT);
  }
  kill(activator_pid, SIGCONT);
  kill(fuel_pid, SIGCONT);
  kill(inhibitor_pid, SIGCONT);
  return 0;
}