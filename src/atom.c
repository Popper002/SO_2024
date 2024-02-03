#include "header/atom.h"
#include "header/common.h"
#include <sys/param.h>

struct atom atom;

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

  int n_atomi_init = atoi(argv[1]);
  int energy_demand = atoi(argv[2]);
  int n_atom_max = atoi(argv[3]);
  int min_a_atomico = atoi(argv[4]);
  int n_nuovi_atomi = atoi(argv[5]);
  int sim_duration = atoi(argv[6]);
  int energy_explode_threshold = atoi(argv[7]);
  int atom_number = atoi(argv[8]);
  argv[9] = NULL;

  config.N_ATOMI_INIT = n_atomi_init;
  config.ENERGY_DEMAND = energy_demand;
  config.N_ATOM_MAX = n_atom_max;
  config.MIN_A_ATOMICO = min_a_atomico;
  config.N_NUOVI_ATOMI = n_nuovi_atomi;
  config.SIM_DURATION = sim_duration;
  config.ENERGY_EXPLODE_THRESHOLD = energy_explode_threshold;
  atom.atomic_number = atom_number;
  printf("[ATOM %d] {FETCHED ARGV COMPLEATE\n}");
}
int get_command() { int comand; }

int energy_from_fission(int atomic_number1, int atomic_number2) 
{
  return atomic_number1 * atomic_number2 -  MAX(atomic_number1, atomic_number2);
}

pid_t atom_fission(int atomic_number, int comand, struct config *config)
{
  pid_t atom_master;
  pid_t atom_child;
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
      int new_atomic_n = atomic_number / 2;
      sleep(1);
      energy_from_fission(new_atomic_n,new_atomic_n);

    default:
      sleep(1);
      break;
    }
  }
}
int main(int argc, char const *argv[])
{
  printf("HELLO IS ATOM %d\n", getpid());
  fetch_args_atom(argv);
  int atomic_number = atoi(argv[8]);
  atomic_number = atom.atomic_number;
  printf("%d",atomic_number);

  print_para_TEST(config);
  return 0;
}
