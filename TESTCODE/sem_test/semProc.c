/* 
_BETA_V_1.0_ 
Primo esempio di uso dei semafori, ho costruto una simulazione in cui abbiamo due risorse da sfruttare in successione, e per evitare sofraffollamenti 
in video ho deciso di creare un semaforo che gestisce la situazione, l'evento in questione è stato strutturato in questo modo: 
    -NUM_PROC :Numero prestabilito di fork da effettuare e di processi che consumeranno 
    -ID_OP1 :Identificatore di una operazione ,un operazione corrisponde ad una risorsa da gestire in semop 
    -ID_OP2 : uguale a ID_OP1 ma con la risorsa 2; 
    -KEY : chiave identificativa della risorsa IPC ma fatta in modo easy (si puô benissimo usare ftok o altro genere di chiave ).
    -myop[2]: array di operazioni da effettuare su array di semafori,2 op array dimensione 2. 

*/



#include "library.h"
#define NUM_PROC 4
#define KEY 2121
#define ID_OP1 1
#define ID_OP2 2
struct sembuf my_op[2];
static void printer() { printf(  "\nBUON_SEEE PID %d\n",getpid()); }
static void rand_print(){srand(time(NULL)); int randomix = rand()%100;printf("\nRANDOMIX FOR PID %d , IS %d\n",getpid(),randomix); }
int main(int argc, char const *argv[])
{
  int sem = semget(KEY, 2, IPC_CREAT);
  printf("SEM %d <OK!>\n",sem);
  semctl(sem, 0, SETVAL, 1);
  for(int i =0 ; i< NUM_PROC ;i++)
  {
    pid_t proc = fork();
    switch (proc)
    {
    case -1: 
        printf("%s\n",errno);
        exit(EXIT_FAILURE); 
        break;
    case 0 /* constant-expression */:
      /* code */
      my_op[0].sem_flg = 0;
      my_op[0].sem_num = ID_OP1;
      my_op[0].sem_op = -1;
      semop(sem,&my_op,1);
      printer();
      my_op[0].sem_num = ID_OP1;
      my_op[0].sem_op = 1;
      semop(sem,&my_op,1);
      printf("FINITO PRINTER PID %d\n", getpid());
      my_op[1].sem_num = ID_OP2;
      my_op[1].sem_op=-1;
      semop(sem,&my_op,1);
      rand_print();
      my_op[1].sem_num = ID_OP2;
      my_op[1].sem_op=1;
      semop(sem,&my_op,1);
      printf("PROC %d FINITO RAND_PRINT\n",getpid()); 
      break;

    default:
            sleep(2); 
      break;
    }
  }
  semctl(sem,IPC_RMID,NULL); 
  printf("REMOVED SEM ID %d\n",sem); 
  exit(EXIT_SUCCESS);
  return 0;
}
