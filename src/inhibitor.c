#include "header/common.h"
#include "header/ipc.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <unistd.h>
int msg_id, stat_id;
struct mes inhibitor_send;
struct message inhibitor_stats_send;
struct config config;
pid_t inhibitor_pid;
struct statistics *inhibitor_stats;
static bool running_flag = true;
/* Possibile idea :
    questo processo pusha in message queue ogni tot nanosecondi degli zeri ,in
   modo da limitare le fissioni il tutto sincronizzato con l'attivatore in modo
   da avere una queue con più quindi la prob di non avere una fissione sarà
    P(FISSIONE) = P(1)|P(INEBITORE) * P(1) * P(ATTIVATORE) /  */
int fission_flag() { return rand() % 2; }


/**
 * @brief take the value from shared memory with key ENERGY_ABSORBED_KEY, do a
 * substraction and send it in msgque to master process for energy_absorbed
 * statistic
 */
void energy_absorbed_value()
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
  int energy_absorbed = *shm;
  energy_absorbed -= config.ENERGY_DEMAND;
  inhibitor_stats_send.m_type = 5;
  inhibitor_stats_send.data = energy_absorbed;

  printf("value of energy absorbed in shared memory: %d\n", energy_absorbed);
  if (msgsnd(stat_id, &inhibitor_stats_send,
	     sizeof(inhibitor_stats_send) - sizeof(long), 0) < 0)
  {
    fprintf(stderr, "%s %s ,ERRNO:%s PID=%d\n", __FILE__, __func__,
	    strerror(errno), getpid());
    exit(EXIT_FAILURE);
  }
}



void fetch_args_inhibitor(char const *argv[])
{

  int n_atom_init = atoi(argv[1]);
  int energy_demand = atoi(argv[2]);
  int n_atom_max = atoi(argv[3]);
  int min_a_atomico = atoi(argv[4]);
  int n_nuovi_atomi = atoi(argv[5]);
  int sim_duration = atoi(argv[6]);

  int energy_explode_threshold = atoi(argv[7]);
  int config_step = atoi(argv[9]);
  argv[10] = NULL;

  config.N_ATOMI_INIT = n_atom_init;
  config.ENERGY_DEMAND = energy_demand;
  config.N_ATOM_MAX = n_atom_max;
  config.MIN_A_ATOMICO = min_a_atomico;
  config.N_NUOVI_ATOMI = n_nuovi_atomi;
  config.SIM_DURATION = sim_duration;
  config.ENERGY_EXPLODE_THRESHOLD = energy_explode_threshold;
  config.STEP = config_step;
}
void signal_handle(int signum)
{
  if (signum == SIGINT)
  {
    if (running_flag)
    {
      write(STDOUT_FILENO, "STOPPED INHIBITOR\n", 19);
      running_flag = false;
    }
    else
    {
      write(STDOUT_FILENO, "STARTED INHIBITOR\n", 19);
      running_flag = true;
    }
  } else if(signum == SIGALRM)
  {
    energy_absorbed_value();
    alarm(config.STEP);
  }
}

int main(int argc, char const *argv[])
{
  
   (void) argc;
  int inhibitor_command = 0;
  int balance = 0;
  inhibitor_pid = getpid();
  signal(SIGINT, signal_handle);
  signal(SIGUSR2, signal_handle);
  
  fetch_args_inhibitor(argv);
  /* idea sarebbe quella che inibitore insieme a attivatore in modo sincrono
    inseriscono dentro la message queue i dati della fissione
    siccome inebitore ha il compito di ridurre le fissioni pushiamo molti piû
    comandi anti fissione in queue e poi gli atomi pescano dentro questa queue e
    chi preleva questi parametri sapra se fare o meno la fissione */

  msg_id =
      msgget(ATOMIC_KEY, IPC_CREAT | 0666); /* @return the same queue of atom */
  stat_id = msgget(STATISTICS_KEY,
		   IPC_CREAT | 0666); /* @return the same queue of atom */
  inhibitor_send.m_type = 1;
  if (msg_id < 0)
  {
    fprintf(stderr, "INHIBITOR : ERROR IN MSGGET <ERRNO : %s> \n",
	    strerror(errno));
    exit(EXIT_FAILURE);
  }
  
    kill(getpid(),SIGSTOP);
   signal(SIGALRM, signal_handle);
   alarm(config.STEP);
  while (1)
  {
    for (int i = 0; i < config.N_ATOMI_INIT + config.N_NUOVI_ATOMI; i++)
    {

      
      // convert command in to string ,inside the msg_buffer
      inhibitor_send.m_type = 1;
      sprintf(inhibitor_send.text, "%d", inhibitor_command);
      msgsnd(msg_id, &inhibitor_send, sizeof(inhibitor_send) - sizeof(long), 0);
  
      if (inhibitor_command == 0)
      {
	balance++;
	inhibitor_stats_send.data = balance;
	inhibitor_stats_send.m_type = 7;
	msgsnd(stat_id, &inhibitor_stats_send, sizeof(inhibitor_stats_send), 0);
      }
    }
  }
  return 0;
}
