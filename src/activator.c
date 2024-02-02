#include "header/common.h"
#include "header/atom.h"
#include <sys/ipc.h>
#include <sys/msg.h>
#include "header/ipc.h"
struct message send; 
struct atom atom;

/* activator is a message queue*/

static int randomic_activation( )
{
    int comand; 
    comand = rand()%2;
    return comand; 
}
void fetch_args(char const *argv[])
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
    printf("[Activator %d] {FETCHED ARGV COMPLEATE\n}");
}
int main(int argc, char const *argv[])
{
    srand(time(NULL));
    printf("HELLO I'M ACTIVATOR %d\n",getpid());       
    int q_id;
    fetch_args(argv);
    q_id = msgget(ATOMIC_KEY , IPC_CREAT|ALL); 
    printf("QUEUE %d CREATED \n ",q_id); 
    send.m_type=1;
    int comand = randomic_activation();
    printf("command %d\n",comand);
    sprintf(send.text,"%d",comand); 
    msgsnd(q_id ,&send ,sizeof(send),0);
    printf("SENDED THIS MESSAGGE %sIN QUEUE%d\n",send.text ,q_id);
    return 0;
}
