#include "header/atom.h"
#include "header/common.h"
#include "header/ipc.h"
#include "util/hash_table.h"
#include "util/my_sem_lib.h"
#include "util/shared_memory.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <unistd.h>
#define CONFIG_PATH "src/config/config1.txt"

char **args_atom[100];
char **activator_args[100];
char **fuel_args[100];
char **inebitore_args[100];

struct statistics *shared_data;
struct atom atom_stat;
// char const *args_[100];
static int shm_id;
static int fork_fuel; 
static int fork_activator; 
static int fork_atom;
static int fork_inhibitor;
static int total_energy ;
static  int id;
shm_fuel *rcv_pid;
int *atom_array_pid;
static key_t key_shm;
pid_t atom_pid;
static pid_t inhibitor_pid;
pid_t inhiitor_pid;
pid_t activator_pid;
pid_t fuel_pid;
struct config config;
struct hash_table table;
enum term_reason term_reason;
struct statistics *shared_data;

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
  #ifdef _PRINT_TEST
  printf("Reading data from file...\n");
  #endif
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
  #ifdef _PRINT_TEST
  printf("Data read from file!\n");
  if (config.INHIBITOR == 1)
  {
    printf("Inhibitor is going to be used in this simulation\n");
  }
  #endif

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

pid_t inhibitor(void)
{
  switch (inhiitor_pid = fork())
  {
  case -1:
    // why_term(MELTDOWN);
  case 0:
    fork_inhibitor++;
    argument_creator((char **)inebitore_args);
    execvp(INHIBITOR_PATH, (char **)inebitore_args);

    fprintf(stderr, "%s line: %d [master %s Problem in execvp with pid %d \n",
	    __func__, __LINE__, __FILE__, getpid());
    exit(EXIT_FAILURE);
    break;

  default:
    return inhiitor_pid;
    break;
  }
}
pid_t fuel_generator(void)
{
  pid_t fuel_pid;
  switch (fuel_pid = fork())
  {
  case -1:
    // why_term(MELTDOWN);
    TEST_ERROR;
    exit(EXIT_FAILURE);
  case 0:
  fork_fuel++; 
#ifdef _PRINT_TEST
    printf("fuel case 0\n");
#endif
    printf("fuel generator pid %d\n", getpid());
    fuel_argument_ipc((char **)fuel_args);
    printf("[%s %s %d] got argument ipc\n", __func__, __FILE__, __LINE__);
    execvp(FUEL_PATH, (char *const *)fuel_args);
    fprintf(
	stderr,
	"%s LINE: %d[MASTER %d  , FUEL_GENERATOR(){PROBLEM IN EXECVP}, %s\n",
	__func__, __LINE__, getpid(), strerror(errno));
    exit(EXIT_FAILURE);
    break;

  default:

    //kill(fuel_pid, SIGSTOP);
    return fuel_pid;
    break;
  }
}

pid_t atom_gen(void)
{
  switch (atom_pid = fork())
  {
  case -1:
    // why_term(MELTDOWN);
    TEST_ERROR;
    exit(EXIT_FAILURE);
  case 0:

    argument_creator((char **)args_atom);
    execvp(ATOM_PATH, (char **)args_atom);

    fprintf(stderr, "%s line: %d[master %s Problem in execvp with pid %d \n",
	    __func__, __LINE__, __FILE__, getpid());
    exit(EXIT_FAILURE);
    break;

  default:

    // kill(atom_pid, SIGSTOP);
#ifdef _PRINT_TEST
    printf("MASTER _ FATHER %d\n", getppid());
#endif
    return atom_pid;
    break;
  }
}
pid_t activator(void)
{

  switch (activator_pid = fork())
  {
  case -1:
    // why_term(MELTDOWN);
    TEST_ERROR;
    exit(EXIT_FAILURE);
  case 0:
  fork_activator++; 
#ifdef _PRINT_TEST
    printf("activator case 0\n");
#endif

    argument_creator((char **)activator_args);
    execvp(ACTIVATOR_PATH, (char **)activator_args);
    printf("activator executed\n");
    fprintf(stderr, "in: %s line: %d[master %d--> problem in execvp %s}\n",
	    __func__, __LINE__, getpid(), strerror(errno));
    exit(EXIT_FAILURE);
    break;

  default:
    kill(activator_pid, SIGSTOP);
    /*waitpid(activator_pid, NULL, 0);*/
#ifdef _PRINT_TEST
    printf("activator case default\n");
#endif
    return activator_pid;
    break;
  }
}

// TODO remove
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


/*
void shutdown()
{
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
}
*/

void store_pid_atom()
{
  atom_array_pid = (int *)malloc(config.N_ATOMI_INIT * sizeof(pid_t));
  if (atom_array_pid == NULL)
  {
    fprintf(stdout, "malloc error %s", strerror(errno));
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < config.N_ATOMI_INIT; i++)
  {
    atom_array_pid[i] = atom_gen();
#ifdef _PRINT_TEST
    printf("[MASTER %d ] %s , [PID %d ] [POS %d]\n", getpid(), __func__,
	   atom_array_pid[i], i);
#endif
  }

 #ifdef _PRINT_TEST
  fprintf(stdout, "Child process %d created and suspended.\n", atom_pid);
  #endif
  free(atom_array_pid);
}

void remove_ipc()
{
  int remove_queue;
  remove_queue = msgget(ATOMIC_KEY, IPC_CREAT); /* get the id for the remove */
  msgctl(remove_queue, IPC_RMID, NULL);
  cleanup_shared_memory();
  fprintf(stdout, "REMOVED ALL IPC'ITEM\n");
}

void handle_signal(int signum)
{
  switch (signum)
  {
  case SIGINT:
    write(STDOUT_FILENO, "SIGINT_HANDLE\n", 15);
    size_t length = sizeof(atom_array_pid) / sizeof(atom_array_pid[0]);
    for (size_t pid = 0; pid < length; pid++)
    {
      kill(atom_array_pid[pid], SIGINT);
    }
    break;
  case SIGUSR1:
    break;
  case SIGALRM:
    write(STDOUT_FILENO, "\n\t-----------------------------------\n", 39);
    write(STDOUT_FILENO, "\t\tALARM : IT'S TIME TO STOP\n", 29);
    write(STDOUT_FILENO, "\n\t-----------------------------------\n", 39);
    remove_ipc();
    killpg(atom_array_pid, SIGKILL);
    killpg(activator_pid, SIGKILL);
    killpg(inhibitor_pid, SIGKILL);
    killpg(fuel_pid, SIGKILL);
    killpg(rcv_pid->array, SIGKILL);
    // total_print();
    
    write(STDOUT_FILENO, "TEARM_REASON < TIMEOUT >\n", 26);
    exit(EXIT_SUCCESS);

    break;
  default:
    break;
  }
}

int why_term(enum term_reason term_reason)
{
  switch (term_reason)
  {
  case EXPLODE:
    fprintf(stderr, "TOO MUCH ENERGY -TERMINATION\n");
    remove_ipc();
    for (int i = 0; i < config.N_ATOM_MAX; i++)
    {
      kill(atom_array_pid[i], SIGINT);
    }
    kill(inhiitor_pid, SIGINT);
    kill(activator_pid, SIGINT);

    exit(EXIT_FAILURE);
    break;
  case TIMEOUT:
    fprintf(stdout, "TIMEOUT -TERMINATION\n");
    for (int i = 0; i < config.N_ATOM_MAX; i++)
    {
      kill(atom_array_pid[i], SIGINT);
    }
    kill(inhiitor_pid, SIGINT);
    kill(activator_pid, SIGINT);

    exit(EXIT_SUCCESS);
    break;
  case BLACKOUT:
    fprintf(stderr, "BLACKOUT- NOT ENOUGH ENERGY\n");
    remove_ipc();
    for (int i = 0; i < config.N_ATOM_MAX; i++)
    {
      kill(atom_array_pid[i], SIGINT);
    }
    kill(inhiitor_pid, SIGINT);
    kill(activator_pid, SIGINT);

    kill(getpid(), SIGINT);
    exit(EXIT_FAILURE);
    break;
  case MELTDOWN:
    fprintf(stderr, "MELTDOWN - FORK-ERROR -TERMINATION\n");
    remove_ipc();
    for (int i = 0; i < config.N_ATOM_MAX; i++)
    {
      kill(atom_array_pid[i], SIGINT);
    }
    kill(inhiitor_pid, SIGINT);
    kill(activator_pid, SIGINT);

    kill(getpid(), SIGINT);
    exit(EXIT_FAILURE);
    break;
  default:
    return term_reason;
    break;
  }
}


/*
void ipc_init()
{

  if (sem_set_val(sem_id, 0, config.N_ATOMI_INIT) == -1)
  {
  fprintf(stderr, "[%s]Error %d in sem_set_val %s\n", __FILE__, errno,
    strerror(errno));
  }
}
*/

void start_atom()
{
  printf("\033[1;32m starting atom as last process \033[0m\n");
  for (int i = 0; i < config.N_ATOMI_INIT; i++)
  {
    kill(atom_array_pid[i], SIGCONT);
#ifdef __PRINT_TEST
    printf("\n\tSTART ATOM %d\n", atom_array_pid[i]);
#endif
  }
  /*@Popper002 TODO  SEGFAULT HERE
  for(int j=0;j<config.N_NUOVI_ATOMI;j++)
  {
    kill(rcv_pid->array[j],SIGCONT);
    printf("\n\tSTART ATOM %d\n", atom_array_pid[j]);

  }
  */
}


void total_print(void)
{
  printf( "\rTOTAL ACTIVATION\t%d\n", shared_data->total_num_activation);
  printf( "\rTOTAL FISSION\t%d\n", shared_data->total_num_fission);
  printf( "\rTOTAL ENERGY PRODUCED\t%d\n", shared_data->energy_produced_value);
  printf( "\rTOTAL ENERGY CONSUMED\t%d\n", shared_data->energy_absorbed);
  printf( "\rTOTAL NUCLEAR WASTE\t%d\n", shared_data->total_nuclear_waste);
  printf( "\rTOTAL ENERGY INHIBITOR CONSUMED\t%d\n", total_energy);
  printf( "\rINHIBITOR PUSHED\t%d\n", shared_data->inhibitor_balancing);
  printf( "\rACTIVATOR PUSHED\t%d\n", shared_data->activator_balancing);



}
void print_last_sec()
{
  // TODO all of this print are place holder
  printf( "PROC\tPID\tENERGY\tN_FORK\tATOMIC NUMBER\tSTATUS\n");
  printf( "MASTER\t%d\t%p\t%p\t%p\t%s\n", getpid(), NULL, NULL, NULL,
	 "OK");
  printf( "\rFUEL\t%d\t%p\t%d\t%p\t%s\n", fuel_pid, NULL,
	 config.N_NUOVI_ATOMI, NULL, "OK");
  printf( "ACTIVATOR\t%d\t%p\t%p\t%p\t%s\n", activator_pid, NULL, NULL,
	 NULL, "OK");
  printf( "INHIBITOR\t%d\t%p\t%p\t%p\t%s\n", inhibitor_pid, NULL, NULL,
	 NULL, "OK");
  printf( "STATS\n");


  printf( "\r TOTAL ENERGY CONSUMED\t%d\n", shared_data->total_num_energy_consumed);

  
  printf("\n\n\n");
  printf( "\rLAST SEC TOTAL ACTIVATION\t%d\n", shared_data->num_activation_last_sec);
  printf( "\rLAST SEC TOTAL FISSION\t%d\n",    shared_data->num_fission_last_sec);
  printf( "\rLAST SEC TOTAL ENERGY PRODUCED\t%d\n", shared_data->total_num_energy_produced_last_sec);
  printf( "\rLAST SEC TOTAL ENERGY CONSUMED\t%d\n", shared_data->num_energy_consumed_last_sec);
  printf( "\rLAST SEC TOTAL NUCLEAR WASTE\t%d\n", shared_data->total_nuclear_waste_last_sec);
  printf( "\rLAST SEC TOTAL ENERGY INHIBITOR CONSUMED\t%d\n", shared_data->num_energy_consumed_inhibitor_last_sec);
  printf("--------------------------------------------------------------\n");
  printf("\n");
  sleep(1);
}

void logo()
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

   printf(
"              _             _         _                 _       _   _              \n"
"     | |           | |       (_)               | |     | | (_)             \n"
"  ___| |_ __ _ _ __| |_   ___ _ _ __ ___  _   _| | __ _| |_ _  ___  _ __   \n"
" / __| __/ _` | '__| __| / __| | '_ ` _ \\| | | | |/ _` | __| |/ _ \\| '_ \\  \n"
" \\__ \\ || (_| | |  | |_  \\__ \\ | | | | | | |_| | | (_| | |_| | (_) | | | | \n"
" |___/\\__\\__,_|_|   \\__| |___/_|_| |_| |_|\\__,_|_|\\__,_|\\__|_|\\___/|_| |_| \n"
"                                                                           \n"
"                                                                           \n"
    );
}
int main(void)
{

  struct statistics *energy_released;
  int start;
  key_shm = KEY_SHM; // ftok("header/common.h",'s');
  void *rcv_ptr;
  init_shared_memory();
#ifdef _PRINT_TEST
  printf("KEY IS %d \n", key_shm);
#endif

  if (key_shm < 0)
  {
    fprintf(stderr, "PROBLEM KEY\n");
  }
  shm_id = shmget(key_shm, sizeof(config.N_ATOMI_INIT) * sizeof(pid_t),
		  IPC_CREAT | 0666);

  srand(time(NULL));
  signal(SIGUSR1, handle_signal);
  signal(SIGALRM, handle_signal);

  //printf("-> Main %d <-\n", getpid());
  scan_data();

  // ipc_init();

#ifdef _PRINT_TEST
  print_para_TEST(config);
#endif

  args_atom[0] = (char **)ATOM_PATH;
  activator_args[0] = (char **)ACTIVATOR_PATH;
  fuel_args[0] = (char **)FUEL_PATH;
  inebitore_args[0] = (char **)INHIBITOR_PATH;

  activator_pid = activator();
  //  kill(activator_pid, SIGSTOP);
#ifdef _PRINT_TEST
  printf("[MASTER %d ] [%s ] [ACTIVATOR PID %d ]\n", getpid(), __func__,
	 activator_pid);
#endif

  fuel_pid = fuel_generator();
  //  kill(fuel_pid, SIGSTOP);
  if (config.INHIBITOR == 1)
  {
    inhibitor_pid = inhibitor();
    //  kill(inhibitor_pid, SIGSTOP);
  }
  store_pid_atom();
  rcv_pid = (shm_fuel *)shmat(shm_id, NULL, 0);

#ifdef _PRINT_TEST
  /* FIXME: here we have the seg fault  */
  for (int i = 0; i < config.N_NUOVI_ATOMI; i++)
  {

    printf("[%s]Il valore memorizzato è %d\n", __FILE__, rcv_pid->array[i]);
  }
  fprintf(stdout, "atoms generated and stopped\n");
#endif


  // shutdown();
  #ifdef _PRINT_TEST
  printf("\n\t\t\tMaster process didn't kill himself :)\n\n");
  printf("\n\t-----------------------------------\n");
  printf("\t\tEverything is ready to start the simulation\n");
  printf("\n\t-----------------------------------\n");
  printf("\n\t\t\tMaster process didn't kill himself :)\n\n");
  #endif
  logo();
  for (start = 10; start > 0; start--)
  {
    printf("\rStarting the simulation in %d...\n", start);
    sleep(1);
  }
  fprintf(stdout, "Master: [PID %d] is starting the simulation\n", getpid());
  alarm(config.SIM_DURATION);

  shmdt(rcv_pid);

  start_atom();



  while (1)
  {
     id = shmget(STATISTICS_KEY,sizeof(struct statistics),IPC_CREAT|0666);
    #ifdef _PRINT_DEBUG
    fprintf(stdout , "SHARED MEMORY ID STATISTICS : %d\n",id); 

    #endif
    rcv_ptr = shmat(id, NULL,0);
    shared_data = (struct statistics*) rcv_ptr; 
    fprintf(stdout , "[MASTER ----] TOTAL ENERGY SH_MEM %d \n", shared_data->total_num_energy_consumed);
//     TODO call a function that displays statistic
     print_last_sec();
  }

  return 0;
} 