#include "header/atom.h"
#include "util/shared_memory.h"

struct atom atom;
static struct message rcv;
struct config config;
struct statistics atom_stats;
static void print_para_TEST(struct config config)
{
  printf("\t\n----------\n");
  printf("N_ATOMI_INIT: %d\n"
	 "ENERGY_DEMAND :%d\n"
	 "N_ATOM_MAX:%d\n"
	 "MIN_A_ATOMICO :%d\n"
	 "N_NUOVI_ATOMI :%d\n"
	 "SIM_DURATION :%d\n"
	 "ENERGY_EXPLODE_THRESHOLD :%d\n"
	 "ATOMIC_NUMBER %d\n",
	 config.N_ATOMI_INIT, config.ENERGY_DEMAND, config.N_ATOM_MAX,
	 config.MIN_A_ATOMICO, config.N_NUOVI_ATOMI, config.SIM_DURATION,
	 config.ENERGY_EXPLODE_THRESHOLD, atom.atomic_number);
  printf("\t\n----------\n");
}

void fetch_args_atom(char const *argv[])
{
  int n_atom_init = atoi(argv[1]);
  int energy_demand = atoi(argv[2]);
  int n_atom_max = atoi(argv[3]);
  int min_a_atomico = atoi(argv[4]);
  int n_nuovi_atomi = atoi(argv[5]);
  int sim_duration = atoi(argv[6]);
  int energy_explode_threshold = atoi(argv[7]);
  argv[8] = NULL;

  config.N_ATOMI_INIT = n_atom_init;
  config.ENERGY_DEMAND = energy_demand;
  config.N_ATOM_MAX = n_atom_max;
  config.MIN_A_ATOMICO = min_a_atomico;
  config.N_NUOVI_ATOMI = n_nuovi_atomi;
  config.SIM_DURATION = sim_duration;
  config.ENERGY_EXPLODE_THRESHOLD = energy_explode_threshold;
#ifdef _PRINT_TEST
  printf("[ATOM %d] {FETCHED ARGV COMPLETED}\n", getpid());
#endif
}

static int energy_free(int atomic_a1, int atomic_a2)
{
  return atomic_a1 * atomic_a2 - MAX((int)atomic_a1, (int)atomic_a2);
}

void atom_fission(struct atom *atom, int command, struct config config)
{

  atom_stats.total_nuclear_waste = 0;
  int child1_atomic_number, child2_atomic_number;
  if (atom->atomic_number <= config.MIN_A_ATOMICO)
    fprintf(stdout, "Starting fissioning atom....\n");
  {
    fprintf(stderr, "Atom with %d as atomic number can't be fissioned\n",
	    atom->atomic_number);
    atom_stats.total_nuclear_waste++;
    update_shared_memory(&atom_stats);
  }
  if (atom->atomic_flag == 1)
  {

    pid_t atom_child = fork();
    switch (atom_child)
    {
    case -1:
      TEST_ERROR
      exit(EXIT_FAILURE);
    case 0:
      atom_stats.num_activation_last_sec++;
      child1_atomic_number = rand() % (atom->atomic_number - 1) +
			     1; // -1 and +1 so we are sure to not exceed the
				// starting atomic number
      child2_atomic_number = atom->atomic_number - child1_atomic_number;

#ifdef _PRINT_TEST
      printf("child1 atomic number %d\n", child1_atomic_number);
      printf("child2 atomic number %d\n", child2_atomic_number);
#endif

      int energy_released =
	  energy_free(child1_atomic_number, child2_atomic_number);
      printf("energy released %d\n", energy_released);
      update_shared_memory(&atom_stats);
      printf("\r[%s %d] fissioned into %d and %d, energy released is %d\n",
	     __FILE__, getpid(), child1_atomic_number, child2_atomic_number,
	     energy_released);
      exit(0);
      break;

    default:
#ifdef _PRINT_TEST
      printf("father process, TODO\n");
#endif

      break;
    }
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
  srand(time(NULL));
  static int command;
  atom.pid = getpid();
#ifdef _PRINT_TEST
  printf("HELLO IS ATOM %d\n", atom.pid);
#endif

  fetch_args_atom(argv);

  init_shared_memory();
  int total_energy = 0;

  rcv.m_type = 1;
  int rcv_id = msgget(ATOMIC_KEY, IPC_CREAT | 0666);
#ifdef _PRINT_TEST
  printf("[%s] connecting to queue:%d\n", __FILE__, rcv_id);
#endif
  if (rcv_id == -1)
  {
    fprintf(stderr, "error in rcv_id queue %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }
  kill(atom.pid, SIGSTOP); // Send Sigstop signal to atom
  if (msgrcv(rcv_id, &rcv, sizeof(rcv) - sizeof(long), 1, IPC_NOWAIT) <= -1)
  {
    fprintf(stderr, "%s Error in msg_rcv\n", __FILE__);
  }

  // Assegna la stringa ricevuta al membro appropriato della struct atom
  // print_para_TEST(config);
#ifdef _PRINT_TEST
  printf("STRINGA RICEVUTA: ID:%d , TYPE :%ld <DATA: %s > \n", rcv_id,
	 rcv.m_type, rcv.text);
  fflush(stdout);
#endif
  atom.atomic_flag = atoi(rcv.text);
  /*
#ifdef _PRINT_TEST
  printf("ATOM FLAG IS %d FOR ATOM %d\n", atom.atomic_flag, atom.pid);
  printf("atom.atomic_number %d\n", atom.atomic_number);
#endif
  */
  
  atom.atomic_number = get_atomic_number();
  fprintf(stdout,"The atomic number of atom [%d] is %d \n", atom.pid,
	 atom.atomic_number);
  
  atom_fission(&atom, atom.atomic_flag, config);
  while (1)
  {
    int energy_released = read_shared_memory();
    total_energy += energy_released;
    atom_stats.total_num_fission +=atom_stats.num_fission_last_sec;
    update_shared_memory(&atom_stats);
  }
 /* cleanup_shared_memory(); */ 
 /* Never lunched this function ,this is caused from the while loop never end's*/

  return 0;
}