#include "header/atom.h"
#include <sys/ipc.h>
#include <sys/msg.h>
#include "header/ipc.h"
#include"header/common.h"
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
static void get_comand(struct atom *atom,struct message rcv )
{
   
}

static void energy_free(){}
pid_t atom_fission(int atomic_number, int comand, struct config *config)
{
    pid_t atom_master; 
    pid_t atom_child;
    if( comand ==1 )
    { 
        switch (atom_master = fork())
        {
        case -1: 
            TEST_ERROR;
            exit(EXIT_FAILURE); 
            break;
        case 0: 
            atom_child = fork();
            atomic_number/2;
            sleep(1); 
            energy_free(atomic_number,atom_master ,atom_child);
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
    static int command;
    printf("HELLO IS ATOM %d\n",getpid()); 
    fetch_args_atom(argv);
    argv[8] = atom.atomic_number;
    static int rcv_id; 
    rcv.m_type=1; 
    rcv_id = msgget(ATOMIC_KEY,O_READ); /*Setup for only read inside the queue*/
    printf("CONNECTING TO QUEUE %d\n", rcv_id);

    int bytes_read = msgrcv(rcv_id, &rcv, sizeof(rcv) - sizeof(long), 1, 0);
    if (bytes_read == -1) {
        perror("Errore ricezione messaggio");
        return;
    }

    // Assegna la stringa ricevuta al membro appropriato della struct atom
    sprintf(atom.atomic_flag,"%d",rcv.text);
    printf("STRINGA RICEVUTA: %s\n", atom.atomic_flag);
    return 0;
}
