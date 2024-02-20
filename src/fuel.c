#include "header/common.h"
#include "header/ipc.h"
struct config config;
char **new_atom_args[100];
static pid_t atom_new_pid[100];
shm_fuel*new_pid_atom;
static int shm_id;
static key_t shm_key;

void argument_creator(char *argv[])
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

void born_new_atom(pid_t *new_pid)
{
  int j;
  for (j = 0; j < config.N_ATOMI_INIT; j++)
  {
    new_pid[j] = fork();
#ifdef _PRINT_TEST
    printf(" %s %d ,%s\n", __FILE__, getpid(), __func__);
#endif
    // #ifdef _PRINT_TEST
    //  printf("%s %s sem release sem_id: %d \t sem_op: %d\n",__func__,__FILE__,
    //  sem_id, semctl(sem_id, 0, GETVAL));
    // #endif
    // put(&table, atom_pid, random_a_number);
    // print_hash_table(&table);

    if (new_pid == 0)
    {
      argument_creator((char **)new_atom_args);
      execvp(ATOM_PATH, (char **)new_atom_args);

      //printf("[%s] [%s] atomi pid inserted in table %d\n", __FILE__, __func__,
	     //new_pid);
    }
    if (new_pid < 0)
    {
      fprintf(stderr, "%s line: %d[master %s Problem in execvp with pid %d \n",
	      __func__, __LINE__, __FILE__, getpid());
      exit(EXIT_FAILURE);
    }
  }
}
/*
void store__new_pid_atom()
{
  for (int i = 0; i < config.N_ATOMI_INIT; i++)
  {
    new_pid_atom[i] = born_new_atom();
#ifdef _PRINT_TEST
    printf("[MASTER %d ] %s , [PID %d ] [POS %d]\n", getpid(), __func__,
	   new_pid_atom[i], i);
#endif
  }
}
*/ 
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
  argv[10] = NULL;

  config.N_ATOMI_INIT = n_atom_init;
  config.ENERGY_DEMAND = energy_demand;
  config.N_ATOM_MAX = n_atom_max;
  config.MIN_A_ATOMICO = min_a_atomico;
  config.N_NUOVI_ATOMI = n_nuovi_atomi;
  config.SIM_DURATION = sim_duration;
  config.ENERGY_EXPLODE_THRESHOLD = energy_explode_threshold;
  shm_id = ipc_shm_id;
  shm_key = ipc_key_shm;
#ifdef _PRINT_TEST
  printf("[ATOM %d] {FETCHED ARGV COMPLEATE\n}", getpid());
#endif
}
#ifdef _PRINT_TEST

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

#endif

/* inseriamo l'array di pid nuovo in memoria condivisa in modo da essere
 * condivisi con altri processi */
/* La chiave del nuovo segmento di memoria viene passata come argv dal master
con un array di chiavi struttura : master crea nuovo oggetto shmget con chiave
passata come argv in posizione 9 in modo da che [master crea]  [fuel attacca ]
  [fuel scrive ] [ fuel detach ] i nuovi pid vengono letti dal master in modo da
sapere ogni STEP secondi le nuove nascite */
int main(int argc, char const *argv[])
{
#ifdef _PRINT_TEST
  printf("[%s][%s][PID:%d]\n", __FILE__, __func__, getpid());
#endif
  fetch_args_fuel(argv);
#ifdef _PRINT_TEST
  print_ALL_IPC();
  print_para_TEST();
  // value_in_memory();
#endif
  shm_id = shmget(shm_key, sizeof(config.N_NUOVI_ATOMI)*sizeof(pid_t) ,
		  IPC_CREAT | 0666);
  if (shm_id < 0)
  {
    perror("FUEL , PROBLEM SHMGET\n");
    exit(EXIT_FAILURE);
  }
#ifdef _PRINT_TEST
  printf(" [%s] [N_NUOVI_ATOMI : %d ]\n", __FILE__, config.N_NUOVI_ATOMI);
#endif

  // store__new_pid_atom();

  int i = 0;
  //*new_pid_atom->array = (shm_fuel * ) malloc(sizeof(config.N_NUOVI_ATOMI)*sizeof(pid_t)); 
  if (new_pid_atom = (shm_fuel*)shmat(shm_id, NULL, 0) < 0)
  {
    printf("[%s][%s][%d][%s]\n", __func__, __FILE__, getpid(), strerror(errno));
  }
  printf("SH_ID %d CONNECTED\n", shm_id);

  born_new_atom(atom_new_pid);
#ifdef _PRINT_TEST
  for (i ; i< config.N_NUOVI_ATOMI;i++)
  {
    printf("\n[FUEL %d ] %s , [PID %d ] [POS %d]\n", getpid(), __func__,
	   atom_new_pid[i], i);
  } 
#endif
  
/* copiamo l'array di pid in memoria condivisa */
memcpy(new_pid_atom->array, atom_new_pid, sizeof(atom_new_pid));
printf("COPY COMPLEATE\n");
if( shmdt(new_pid_atom )< 0 ) { perror("");}
#ifdef _PRINT_TEST
  stampaStatoMemoria(shm_id);
#endif

  return 0;
}
