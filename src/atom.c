#include "header/atom.h"
#include "header/common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <unistd.h>
struct atom atom;
static struct mes rcv;
struct config config;
int master_pid;
struct statistics stats = {0};
int stat_id;

int nums_bytes_writed;
static struct message send_stats;

void fetch_args_atom(char const *argv[])
{
  int n_atom_init = atoi(argv[1]);
  int energy_demand = atoi(argv[2]);
  int n_atom_max = atoi(argv[3]);
  int min_a_atomico = atoi(argv[4]);
  int n_nuovi_atomi = atoi(argv[5]);
  int sim_duration = atoi(argv[6]);
  int energy_explode_threshold = atoi(argv[7]);
  master_pid = atoi(argv[8]);
  argv[9] = NULL;

  config.N_ATOMI_INIT = n_atom_init;
  config.ENERGY_DEMAND = energy_demand;
  config.N_ATOM_MAX = n_atom_max;
  config.MIN_A_ATOMICO = min_a_atomico;
  config.N_NUOVI_ATOMI = n_nuovi_atomi;
  config.SIM_DURATION = sim_duration;
  config.ENERGY_EXPLODE_THRESHOLD = energy_explode_threshold;
  /* #ifdef _PRINT_TEST
    printf("[ATOM %d] {FETCHED ARGV COMPLETED}\n", getpid());
   #endif */
}

void insert_value_in_shm(int energy_released)
{
  int shm_id = shmget(ENERGY_ABSORBED_KEY, sizeof(int), IPC_CREAT | 0666);
  if (shm_id < 0)
  {
    fprintf(stderr, "%s Error in shmget: %s\n", __FILE__, strerror(errno));
    exit(EXIT_FAILURE);
  }
  int *shm = (int *)shmat(shm_id, NULL, 0);
  if (shm == (void *)-1)
  {
    fprintf(stderr, "%s Error in shmat: %s\n", __FILE__, strerror(errno));
    exit(EXIT_FAILURE);
  }
  *shm = energy_released;
  printf("value of energy released in shared memory: %d",energy_released);
}

static int energy_free(int atomic_a1, int atomic_a2)
{
  return atomic_a1 * atomic_a2 - MAX((int)atomic_a1, (int)atomic_a2);
}

void meltdown_simulation()
{
  write(STDOUT_FILENO, "sending signal SIGUSR1 to master\n", 34);
  kill(master_pid, SIGUSR1);
}

int atom_fission(struct atom *atom, struct config config)
{

  stats.total_nuclear_waste_last_sec = 0;
  int child1_atomic_number =0; 
  int child2_atomic_number = 0;
  int read_atomic_number = 0;
  int fd[2];
  char write_buffer[100], read_buffer[100];
  int pipe_ret = pipe(fd);

  if (pipe_ret < 0)
  {
    fprintf(stderr, "Error opening pipe segment %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }
  if (atom->atomic_number <= config.MIN_A_ATOMICO)
  // fprintf(stdout, "Starting fissioning atom....\n");
  {
    // fprintf(stderr, "Atom with %d as atomic number can't be
    // fissioned\n",atom->atomic_number);
    send_stats.m_type = 3;
    stats.total_nuclear_waste_last_sec++;
    send_stats.data = stats.total_nuclear_waste_last_sec;
    msgsnd(stat_id, &send_stats, sizeof(int), 0);
    // fprintf(stdout, "\nATOM_SEND_STATS ID:%d,<WASTE %s>\n",
    // stat_id,send_stats.text);
    exit(EXIT_SUCCESS);
  }
  if (atom->atomic_flag == 1)
  {
    send_stats.m_type = 8;
    stats.num_fission_last_sec++;
    send_stats.data = stats.num_fission_last_sec;
    msgsnd(stat_id, &send_stats, sizeof(int), 0);

    pid_t atom_child = fork();

    switch (atom_child)
    {
    case -1:
      meltdown_simulation();
      break;
    case 0:
      close(fd[1]);
      send_stats.m_type = 1;
      stats.num_activation_last_sec++;
      send_stats.data = stats.num_activation_last_sec;
      msgsnd(stat_id, &send_stats, sizeof(int), 0);
      read_atomic_number = read(fd[0], read_buffer, sizeof(read_buffer));
      if (read_atomic_number > 0)
      {
	read_buffer[read_atomic_number] = '\0';
      }

      int energy_released =
	  energy_free(child1_atomic_number, child2_atomic_number);
      if (config.INHIBITOR == 1)
      {
	insert_value_in_shm(energy_released);
      }

      send_stats.m_type = 2;
      send_stats.data = energy_released;
      msgsnd(stat_id, &send_stats, sizeof(int), 0);
      close(fd[0]);
      // exit(EXIT_SUCCESS);
      return 0;
      break;

    default:
      child1_atomic_number = rand() % (atom->atomic_number - 1) + 1; // -1 and +1 so we are sure to not exceed thestarting atomic number
      child2_atomic_number = atom->atomic_number - child1_atomic_number;
      sprintf(write_buffer, "%d", child2_atomic_number);
      nums_bytes_writed = write(fd[1], write_buffer, sizeof(write_buffer));
      close(fd[1]);
      break;
    }
  }
  close(fd[0]);
  close(fd[1]);
  return 0;
}

void handler_signal(int signum)
{
  switch (signum)
  {
  case SIGCHLD:
    wait(NULL);
    break;

  default:
    break;
  }
}
/**
 * @brief using the current time in nanosecond as seed, generates a random
 * number between 0 and N_ATOM_MAX
 * @return a random value between 0 and N_ATOM_MAX
 */
int get_atomic_number()
{
  struct timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  unsigned long long seed = (ts.tv_sec * 1000000000 + ts.tv_nsec);
  srand(seed);
  return rand() % config.N_ATOM_MAX;
}

int main(int argc, char const *argv[])
{
  (void)argc;
  srand(time(NULL));
  signal(SIGCHLD, handler_signal);
  atom.pid = getpid();
  stat_id = msgget(STATISTICS_KEY, IPC_CREAT | 0666);
  signal(SIGUSR1, meltdown_simulation);
  if (stat_id < 0)
  {
    fprintf(stderr, "%s Error in msgget: %s>\n", __FILE__, strerror(errno));
    exit(EXIT_FAILURE);
  }
  /* #ifdef _PRINT_TEST
    printf("HELLO IS ATOM %d\n", atom.pid);
   #endif */

  fetch_args_atom(argv);

  rcv.m_type = 1;
  kill(atom.pid, SIGSTOP);

  int rcv_id = msgget(ATOMIC_KEY, IPC_CREAT | ALL);
  /* #ifdef _PRINT_TEST
    printf("[%s] connecting to queue:%d\n", __FILE__, rcv_id);
   #endif */
  if (rcv_id == -1)
  {
    fprintf(stderr, "error in rcv_id queue %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }
  // FIXME: Invalid argument
  if (msgrcv(rcv_id, &rcv, sizeof(rcv) - sizeof(long), 1, 0) < 0)
  {
    fprintf(stderr, "%s Error in msg_rcv: %s\n", __FILE__, strerror(errno));
  }

  // Assegna la stringa ricevuta al membro appropriato della struct atom
  // print_para_TEST(config);
  /*
 #ifdef _PRINT_TEST
  printf("STRINGA RICEVUTA: ID:%d , TYPE :%ld <DATA: %s > \n", rcv_id,
	 rcv.m_type, rcv.text);
  fflush(stdout);
 #endif */

  atom.atomic_flag = atoi(rcv.text);

  /* #ifdef _PRINT_TEST
    printf("ATOM FLAG IS %d FOR ATOM %d\n", atom.atomic_flag, atom.pid);
    printf("atom.atomic_number %d\n", atom.atomic_number);
   #endif */

  atom.atomic_number = get_atomic_number();
  /*fprintf(stdout, "The atomic number of atom [%d] is %d \n", atom.pid,
	  atom.atomic_number);*/

  atom_fission(&atom, config);


 
  
  exit(EXIT_SUCCESS);

  return 0;
}