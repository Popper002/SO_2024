#include "header/common.h"
#include "header/ipc.h"
#include <stdio.h>
#include <stdlib.h>
static int msg_id;
struct message inhibitor_send;
struct config config;
/* Possibile idea :
    questo processo pusha in message queue ogni tot nanosecondi degli zeri ,in
   modo da limitare le fissioni il tutto sincronizzato con l'attivatore in modo
   da avere una queue con più quindi la prob di non avere una fissione sarà
    P(FISSIONE) = P(1)|P(INEBITORE) * P(1) * P(ATTIVATORE) /  */
int fission_flag() { return rand() % 2; }

void fetch_args_inhibitor(char const *argv[])
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
  printf("[INEBITORE %d] {FETCHED ARGV COMPLEATE\n}", getpid());
#endif
}

int main(int argc, char const *argv[])
{
#ifdef _PRINT_TEST

  printf("[%s][%s][PID:%d]\n", __FILE__, __func__, getpid());
#endif
  if (argc < 8)
  {
    fprintf(stderr, "[%s] Not enoough arguments", __FILE__);
    exit(EXIT_FAILURE);
  }
  fetch_args_inhibitor(argv);
  int inhibitor_command;
  /* idea sarebbe quella che inebitore insieme a attivatore in modo sincrono
    inseriscono dentro la message queue i dati della fissione
    siccome inebitore ha il compito di ridurre le fissioni pushiamo molti piû
    comandi anti fissione in queue e poi gli atomi pescano dentro questa queue e
    chi preleva questi parametri sapra se fare o meno la fissione */

  msg_id =
      msgget(ATOMIC_KEY, IPC_CREAT | ALL); /* @return the same queue of atom */
  inhibitor_send.m_type = 1;

#ifdef _PRINT_TEST
  printf("ID %d \n", msg_id);
#endif
  for (int i = 0; i < config.N_ATOMI_INIT + config.N_ATOM_MAX; i++)
  {
    inhibitor_command = fission_flag();
    /* convert command in to string ,inside the msg_buffer*/
    sprintf(inhibitor_send.text, "%d", inhibitor_command);
    // FIXME error in this send
    if (msgsnd(msg_id, &inhibitor_send, sizeof(inhibitor_send) - sizeof(long),
	       0) < 0)
    {
      fprintf(stderr, "%s %s ,ERRNO:%s PID=%d\n", __FILE__, __func__,
	      strerror(errno), getpid());
    }
#ifdef _PRINT_TEST
    printf("[%s][%s][%d][VALUE: %d IN MSG_BUFF:%s]\n", __FILE__, __func__,
	   getpid(), inhibitor_command, inhibitor_send.text);
#endif
  }

  return 0;
}
