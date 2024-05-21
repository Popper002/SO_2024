

#include "header/common.h"
#include "header/ipc.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>

#define NANO_SECOND_MULTIPLIER  1000000 

struct config config;
struct message send_stats;
char **new_atom_args[100];
pid_t *atom_new_pid;
shm_fuel *new_pid_atom;
static int  q_stats; 
pid_t new_atom;
static int shm_id;
static key_t shm_key;
struct timespec sleepValue = {0};
pid_t master_pid;
pid_t *psu_atom_array_pid; 

void atom_argument_creator(char *argv[])
{
  char n_atomi_init[10];
  char energy_demand[10];
  char n_atom_max[10];
  char min_a_atomico[10];
  char n_nuovi_atomi[10];
  char sim_duration[10];
  char energy_explode_threshold[10];
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

pid_t born_new_atom()
{

  // int random_a_number = randomize_atom(config.MIN_A_ATOMICO);
  switch (new_atom = fork())
  {
  case -1:
    kill(master_pid,SIGUSR1);
    break;
  case 0:
/* #ifdef _PRINT_TEST
    printf(" %s %d ,%s\n", __FILE__, getpid(), __func__);
 #endif */
    send_stats.m_type=1;
    statistics_data.num_activation_last_sec++;
    send_stats.data=statistics_data.num_activation_last_sec;
    msgsnd(q_stats, &send_stats, sizeof(int), 0);
    atom_argument_creator((char **)new_atom_args);
    execvp(ATOM_PATH, (char **)new_atom_args);
    fprintf(stderr, "%s line: %d[master %s Problem in execvp with pid %d \n",
	    __func__, __LINE__, __FILE__, getpid());
    exit(EXIT_FAILURE);
    return new_atom;

    break;

  default:

    waitpid(new_atom, NULL, 0);
    return new_atom;
    break;
  }
  return new_atom;
}
 

void fetch_args_fuel(char const *argv[])
{

  int n_atom_init = atoi(argv[1]);
  int energy_demand = atoi(argv[2]);
  int n_atom_max = atoi(argv[3]);
  int min_a_atomico = atoi(argv[4]);
  int n_nuovi_atomi = atoi(argv[5]);
  int sim_duration = atoi(argv[6]);
  int energy_explode_threshold = atoi(argv[7]);
  int ipc_shm_id = atoi(argv[8]);
  key_t ipc_key_shm = atoi(argv[9]);
  int step = atoi(argv[10]); 
  master_pid = atoi(argv[11]);
  argv[12] = NULL;

  config.N_ATOMI_INIT = n_atom_init;
  config.ENERGY_DEMAND = energy_demand;
  config.N_ATOM_MAX = n_atom_max;
  config.MIN_A_ATOMICO = min_a_atomico;
  config.N_NUOVI_ATOMI = n_nuovi_atomi;
  config.SIM_DURATION = sim_duration;
  config.ENERGY_EXPLODE_THRESHOLD = energy_explode_threshold;
  config.STEP = step;
  shm_id = ipc_shm_id;
  shm_key = ipc_key_shm;

  printf("%s master_pid received is: %d\n",__FILE__,master_pid);
/* #ifdef _PRINT_TEST
  printf("[ATOM %d] {FETCHED ARGV COMPLEATE\n}", getpid());
 #endif */
}
/* #ifdef _PRINT_TEST

void print_para_TEST()
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
void print_ALL_IPC()
{
  printf("SHID : %d ] [ STATUS <CREATED> , sizeof(%ld) [KEY :%d]]\n", shm_id,
	 sizeof(shm_id), shm_key);
}
void value_in_memory()
{
  for (int i = 0; i < config.N_NUOVI_ATOMI; i++)
  {
  }
}
int stampaStatoMemoria(int shid)
{

  struct shmid_ds buf;

  if (shmctl(shid, IPC_STAT, &buf) == -1)
  {
    fprintf(stderr, "%s: %d. Errore in shmctl #%03d: %s\n", __FILE__, __LINE__,
	    errno, strerror(errno));
    return -1;
  }
  else
  {
    printf("\nSTATISTICHE\n");
    printf("AreaId: %d\n", shid);
    printf("Dimensione: %ld\n", buf.shm_segsz);
    printf("Ultima shmat: %s\n", ctime(&buf.shm_atime));
    printf("Ultima shmdt: %s\n", ctime(&buf.shm_dtime));
    printf("Ultimo processo shmat/shmdt: %d\n", buf.shm_lpid);
    printf("Processi connessi: %ld\n", buf.shm_nattch);
    printf("\n");
    return 0;
  }
}



void start_atom(pid_t atom)
{
  killpg(atom,SIGCONT); 
}
double step_nanosec()
{
  double retNsec;
  struct timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  retNsec = ts.tv_nsec;
  return retNsec;
}






/
 * inseriamo l'array di pid nuovo in memoria condivisa in modo da essere
 * condivisi con altri processi
 *   La chiave del nuovo segmento di memoria viene passata come argv dal master
 * con un array di chiavi struttura : master crea nuovo oggetto shmget con
 * chiave passata come argv in posizione 9 in modo da che [master crea]  [fuel
 * attacca ] [fuel scrive ] [ fuel detach ] i nuovi pid vengono letti dal master
 * in modo da sapere ogni STEP secondi le nuove nascite
 */


void psu_atom_start()
{
  for(int i =0 ; i < config.N_ATOMI_INIT;i++){
  kill(psu_atom_array_pid[i],SIGCONT);
  }
}
int main(int argc, char const *argv[])
{
  (void) argc;
  srand(time(NULL));
  
  fetch_args_fuel(argv);


  q_stats = msgget(STATISTICS_KEY,IPC_CREAT|0666);
  if(q_stats < 0 ){fprintf(stderr,"[FUEL %d] ERROR MSGGET STATISTICS Q \n",getpid());exit(EXIT_FAILURE);} 
  shm_id = shmget(KEY_SHM, sizeof(config.N_NUOVI_ATOMI) ,IPC_CREAT | 0666);
  if (shm_id < 0)
  {
    fprintf(stderr, "%s, error[%s] in shmget[LINE %d]\n",__FILE__,strerror(errno),__LINE__);
    exit(EXIT_FAILURE);
  }

 
 psu_atom_array_pid = (pid_t *)malloc(config.N_NUOVI_ATOMI * sizeof(int));
  if (psu_atom_array_pid == NULL)
  {
    fprintf(stdout, "malloc error :ERR:%s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

    sleepValue.tv_sec = (config.STEP * NANO_SECOND_MULTIPLIER) / 1000000000;
    sleepValue.tv_nsec = (config.STEP * NANO_SECOND_MULTIPLIER) % 1000000000;
while (1)
{
  for(int i =0 ; i<config.N_NUOVI_ATOMI;i++)
  {

    psu_atom_array_pid[i]=born_new_atom();   
    fprintf(stdout,"PSU GEN ATOM PID %d\n",psu_atom_array_pid[i]); 
  }
   psu_atom_start(); 
   if(nanosleep(&sleepValue,NULL)<0 ) {perror("nanosleep");}
   else {fprintf(stdout,"Sleep for %ld nanosec\n",sleepValue.tv_nsec);
   //free(psu_atom_array_pid);
 }
}


  return 0;
}
