#include "header/atom.h"
#include "header/common.h"
#include "header/ipc.h"
#include <errno.h>
#include <string.h>
#include <sys/cdefs.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/param.h>
struct atom atom;
static struct message rcv;
static void print_para_TEST(struct config config)
{
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
  printf("[ATOM %d] {FETCHED ARGV COMPLEATE\n}", getpid());
}
/* ALPHA _-_*/
static int energy_free(int atomic_a1, int atomic_a2)
{
  return atomic_a1 * atomic_a2 - MAX((int)atomic_a1, (int)atomic_a2);
}
//__-_-_
pid_t atom_fission(int atomic_number, int comand, struct config config)
{
  pid_t atom_master;
  pid_t atom_child;
  struct atom a1;
  struct atom a2;
  if (comand == 1)
  {
    switch (atom_master = fork())
    {
    case -1:
      TEST_ERROR;
      exit(EXIT_FAILURE);
      break;
    case 0:
      atom_child = fork();
      // atomic_number/2;
      sleep(1);
      printf("[%s]first atomic number: %d \\ second atomic number:%d\n",__FILE_NAME__,
	     a1.atomic_number, a2.atomic_number);
      int new_energy = energy_free(a1.atomic_number, a2.atomic_number);
      printf("_------__----_----_-_-\n");
      printf("NEW ATOM PID: %d NEW ATOMIC NUMBER %d\n", atom.pid, new_energy);
    default:
      sleep(1);
      break;
    }
  }
}

static int get_atomic_number()
{
  return rand() % config.N_ATOM_MAX;
}

int main(int argc, char const *argv[])
{
  srand(time(NULL));
  static int command;
  atom.pid = getpid();
  printf("HELLO IS ATOM %d\n", atom.pid);
  fetch_args_atom(argv);
  rcv.m_type = 1;
  int rcv_id = msgget(ATOMIC_KEY, IPC_CREAT | 0666);
  printf("[%s] connecting to queue:%d\n", __FILE__, rcv_id);
  if (rcv_id == -1)
  {
    fprintf(stderr, "error in rcv_id queue %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }
  if (msgrcv(rcv_id, &rcv, sizeof(rcv) - sizeof(long), 1, IPC_NOWAIT) <= -1)
  {
    fprintf(stderr, "ERROR MSG_RCV\n");
  };
  // Assegna la stringa ricevuta al membro appropriato della struct atom
  printf("STRINGA RICEVUTA: ID:%d , TYPE :%ld <DATA: %s > \n", rcv_id,
	 rcv.m_type, rcv.text);
  fflush(stdout);
  atom.atomic_flag = atoi(rcv.text);
  printf("ATOM FLAG IS %d FOR ATOM %d\n", atom.atomic_flag, atom.pid);
  atom.atomic_number = get_atomic_number();
  printf("ATOMIC NUMBER FOR ATOM %d IS %d \n", atom.pid, atom.atomic_number);
  atom_fission(atom.atomic_number, atom.atomic_flag, config);
  return 0;
}
