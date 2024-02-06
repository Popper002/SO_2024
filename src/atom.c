#include "header/atom.h"
#include "header/common.h"
#include "header/ipc.h"
#include <errno.h>
#include <string.h>
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
  int atom_number = atoi(argv[8]);
  argv[9] = NULL;

  config.N_ATOMI_INIT = n_atom_init;
  config.ENERGY_DEMAND = energy_demand;
  config.N_ATOM_MAX = n_atom_max;
  config.MIN_A_ATOMICO = min_a_atomico;
  config.N_NUOVI_ATOMI = n_nuovi_atomi;
  config.SIM_DURATION = sim_duration;
  config.ENERGY_EXPLODE_THRESHOLD = energy_explode_threshold;
  atom.atomic_number = atom_number;
  printf("[ATOM %d] {FETCHED ARGV COMPLEATE\n}",getpid());
}
static void get_comand(struct atom *atom, struct message rcv) {}

/*
int energy_free(int atomic_nuber1) {
  return atomic_nuber1  - atomic_nuber2 - MAX(atomic_nuber1, atomic_nuber2); 
}
*/
void atom_fission(int atomic_number, int comand, struct config *config)
{
  pid_t atom_master;
  pid_t atom_child;
  int nuclear_waste = 0;
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
      // scission();
     int scission_value =  atomic_number / 2; //FIXME doubt that aotmic number should be devided per 2 
      if(atomic_number <= config->MIN_A_ATOMICO){
        kill(getpid(), SIGKILL);
        nuclear_waste++;
      }
      // energy_free(atomic_number);
    default:
      sleep(1);
      break;
    }
  }
}

static int get_atomic_number() {
  srand(time(NULL)); 
  return rand() % config.N_ATOM_MAX; 
  }

int main(int argc, char const *argv[])
{
  static int command;
  printf("HELLO IS ATOM %d\n", getpid());
  fetch_args_atom(argv);
  char atomic_number_str[10];
  sprintf(atomic_number_str, "%d", atom.atomic_number);
  argv[8] = atomic_number_str;
  rcv.m_type = 1;
  int atomic_random_number = get_atomic_number(); 
  printf("atomic_random_number is: %d\n", atomic_random_number);
  int rcv_id = msgget(ATOMIC_KEY, ALL | IPC_CREAT); /*Setup for only read inside the queue*/
  printf("CONNECTING TO QUEUE %d\n", rcv_id); 
  if(rcv_id == -1 ){ 
    fprintf(stderr,"error in rcv_id queue %s\n", strerror(errno));

  }

  int bytes_read = msgrcv(rcv_id, &rcv, sizeof(rcv) - sizeof(long), 1, 0);
  if (bytes_read == -1)
  {
    fprintf(stderr, "Error receiving message %d %s\n",errno, strerror(errno));
    exit(1);
  }

  // Assegna la stringa ricevuta al membro appropriato della struct atom
  printf("STRINGA RICEVUTA: %d %s\n", atom.atomic_flag, rcv.text);
  return 0;
}
