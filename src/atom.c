#include "header/common.h"
#include "header/atom.h"
struct atom atom; 

static void print_para_TEST(struct config config)
{
     printf(
"N_ATOMI_INIT: %d\n"
 "ENERGY_DEMAND :%d\n"
 "N_ATOM_MAX:%d\n"
 "MIN_A_ATOMICO :%d\n"
 "N_NUOVI_ATOMI :%d\n"
 "SIM_DURATION :%d\n"
"ENERGY_EXPLODE_THRESHOLD :%d\n"
 "ATOMIC_NUMBER %d\n",
config.N_ATOMI_INIT,config.ENERGY_DEMAND , config.N_ATOM_MAX,config.MIN_A_ATOMICO
 , config.N_NUOVI_ATOMI,config.SIM_DURATION,config.ENERGY_EXPLODE_THRESHOLD,atom.atomic_number);
}
void fetch_args_atom(char const *argv[])
{

    int n_atomi_init = atoi(argv[1]);
    int energy_demand = atoi(argv[2]);
    int n_atom_max= atoi(argv[3]);
    int min_a_atomico = atoi(argv[4]);
    int n_nuovi_atomi = atoi(argv[5]);
    int sim_duration = atoi(argv[6]);
    int energy_explode_threshold = atoi(argv[7]);
    int atom_number = atoi(argv[8]);
    argv[9] =NULL;
   

    config.N_ATOMI_INIT = n_atomi_init;
    config.ENERGY_DEMAND = energy_demand;
    config.N_ATOM_MAX = n_atom_max;
    config.MIN_A_ATOMICO = min_a_atomico;
    config.N_NUOVI_ATOMI= n_nuovi_atomi;
    config.SIM_DURATION = sim_duration;
    config.ENERGY_EXPLODE_THRESHOLD = energy_explode_threshold;
    atom.atomic_number =atom_number;
    printf("[ATOM %d] {FETCHED ARGV COMPLEATE\n}");
 
}

int main(int argc, char const *argv[])
{
    printf("HELLO IS ATOM %d\n",getpid()); 
    fetch_args_atom(argv);
    argv[8] = atom.atomic_number;
   
    print_para_TEST(config); 
    return 0;
}
