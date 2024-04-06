#include "header/common.h"
#include "header/ipc.h"
struct config config;
char **new_atom_args[100];
pid_t *atom_new_pid;
shm_fuel *new_pid_atom;
pid_t new_atom;
static int shm_id;
static key_t shm_key;

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
    TEST_ERROR;
    exit(EXIT_FAILURE);
  case 0:
#ifdef _PRINT_TEST
    printf(" %s %d ,%s\n", __FILE__, getpid(), __func__);
#endif

    atom_argument_creator((char **)new_atom_args);
    execvp(ATOM_PATH, (char **)new_atom_args);

    printf("[%s] [%s] atomi pid inserted in table %d\n", __FILE__, __func__,
	   new_atom);
    fprintf(stderr, "%s line: %d[master %s Problem in execvp with pid %d \n",
	    __func__, __LINE__, __FILE__, getpid());
    exit(EXIT_FAILURE);
    break;

  default:
    return new_atom;
    break;
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
void signal_handle(int signum)
{
  switch (signum)
  {
  case SIGALRM:
    write(STDOUT_FILENO, "FUEL : FINISH CREATE NEW ATOM'S..BYE\n", 38);
    shmctl(shm_id, IPC_RMID, NULL);
    kill(getpid(), SIGTERM);
    break;

  default:
    break;
  }
}
double step_nanosec()
{
  double retNsec;
  struct timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  retNsec = ts.tv_nsec;
  return retNsec;
}
/**
 * inseriamo l'array di pid nuovo in memoria condivisa in modo da essere
 * condivisi con altri processi
 *   La chiave del nuovo segmento di memoria viene passata come argv dal master
 * con un array di chiavi struttura : master crea nuovo oggetto shmget con
 * chiave passata come argv in posizione 9 in modo da che [master crea]  [fuel
 * attacca ] [fuel scrive ] [ fuel detach ] i nuovi pid vengono letti dal master
 * in modo da sapere ogni STEP secondi le nuove nascite
 */
int main(int argc, char const *argv[])
{
  (void) argc;
  srand(time(NULL));
  signal(SIGALRM, signal_handle);
#ifdef _PRINT_TEST
  printf("[%s][%s][PID:%d]\n", __FILE__, __func__, getpid());
#endif

  
  fetch_args_fuel(argv);
  config.STEP = step_nanosec();
#ifdef _PRINT_TEST
  fprintf(stdout, "NANOSEC VALUE :%ld\n", config.STEP);
#endif
#ifdef _PRINT_TEST
  print_ALL_IPC();
  print_para_TEST();
  fprintf(stdout, "NANOSEC VALUE :%ld\n", config.STEP);
  // value_in_memory();
#endif

  shm_id = shmget(KEY_SHM, sizeof(config.N_NUOVI_ATOMI) * sizeof(pid_t) ,
		  IPC_CREAT | 0666);
  if (shm_id < 0)
  {
    fprintf(stderr, "FUEL , PROBLEM SHMGET\n");
    exit(EXIT_FAILURE);
  }
#ifdef _PRINT_TEST
  printf(" [%s] [N_NUOVI_ATOMI : %d ]\n", __FILE__, config.N_NUOVI_ATOMI);
#endif
  atom_new_pid = (pid_t *)malloc(sizeof(pid_t) * config.N_ATOMI_INIT  );
  if (atom_new_pid == NULL)
  {
    fprintf(stdout, "malloc error %s", strerror(errno));
    exit(EXIT_FAILURE);
  }
  // store__new_pid_atom();

  //*new_pid_atom->array = (shm_fuel * )
  // malloc(sizeof(config.N_NUOVI_ATOMI)*sizeof(pid_t));
  // alarm(config.STEP);
  /* Indecisione o ogni STEP nanosecondi viene interpretato come Fuel runna fino
  a STEP nanosecondi o lo facciamo dormire per STEP nonosecondi finchè non
  finisce la simulazione
  */

  /*Altra idea STEP facciamo randomico numeri floating point compreso tra 0.N e
   * 1,N */

  struct timespec ns_step, sec_step;
  sec_step.tv_sec = config.STEP;
  ns_step.tv_nsec = config.STEP * 1000000000LL;






  while (1)
  {
  for (int i = 0; i < config.N_NUOVI_ATOMI; i++)
  {
    atom_new_pid[i] = born_new_atom();
    nanosleep(&ns_step, &sec_step);

    new_pid_atom = (shm_fuel *)shmat(shm_id, NULL, 0);
  /* copiamo l'array di pid in memoria condivisa */
  memcpy(new_pid_atom->array[i], atom_new_pid, sizeof(pid_t));
  }

#ifdef _PRINT_TEST
  for (int l = 0; l < config.N_NUOVI_ATOMI; l++)
  {
    printf("\n[FUEL %d ] %s , [PID %d ] [POS %d]\n", getpid(), __func__,
	   atom_new_pid[l], l);
  }
#endif
  
#ifdef _PRINT_TEST
  printf("COPY COMPLEATE\n");
#endif
  fflush(stdout);
  if (shmdt(new_pid_atom) < 0)
  {
    fprintf(stderr, "Shared memory ");
   }
  
  }







#ifdef _PRINT_TEST
  stampaStatoMemoria(shm_id);
#endif
 

  return 0;
}
