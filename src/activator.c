//TODO move this in a separate header file
#include "header/atom.h"
#include "header/common.h"
#include "header/ipc.h"
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#include "header/ipc.h"

struct message send;
struct config config;

/* activator is a message queue*/

static int randomic_activation()
{
  int comand;
  comand = rand() % 2;
  return comand;
}
void fetch_args(char const *argv[])
{

  int n_atomi_init = atoi(argv[1]);
  int energy_demand = atoi(argv[2]);
  int n_atom_max = atoi(argv[3]);
  int min_a_atomico = atoi(argv[4]);
  int n_nuovi_atomi = atoi(argv[5]);
  int sim_duration = atoi(argv[6]);
  int energy_explode_threshold = atoi(argv[7]);
  argv[8] = NULL;

  config.N_ATOMI_INIT = n_atomi_init;
  config.ENERGY_DEMAND = energy_demand;
  config.N_ATOM_MAX = n_atom_max;
  config.MIN_A_ATOMICO = min_a_atomico;
  config.N_NUOVI_ATOMI = n_nuovi_atomi;
  config.SIM_DURATION = sim_duration;
  config.ENERGY_EXPLODE_THRESHOLD = energy_explode_threshold;
  printf("[Activator %d] {Fetching arguments compleated}\n",getpid());
}


int main(int argc, char const *argv[])
{
  srand(time(NULL));
  #ifdef _PRINT_TEST
  printf("HELLO I'M ACTIVATOR %d\n", getpid());
  #endif
  static int q_id, i ;
  fetch_args(argv);
  q_id = msgget(ATOMIC_KEY, IPC_CREAT | 0666);
  printf("[%s] QUEUEU : %d CREATED \n ",__FILE__, q_id);
  for( i=0 ; i<config.N_ATOMI_INIT ; i++){
  send.m_type = 1;
  int command = randomic_activation();
  printf("\ncommand %d\n", command);
  sprintf(send.text, "%d", command);
  if(msgsnd(q_id, &send, sizeof(send)-sizeof(long), 0) <=-1){fprintf(stderr,"ERROR IN MSGSND\n");};
  printf("SENDED THIS MESSAGGE %s IN QUEUE %d TYPE:%ld\n", send.text, q_id,send.m_type);
  }
  return 0;
}
