#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int main() {
  // Crea una chiave per il semaforo
  key_t key = ftok(".", 'a');
  if (key == -1) {
    perror("Errore nella creazione della chiave del semaforo");
    return 1;
  }

  // Apre il semaforo
  int sem_id = semget(key, 1, 0);
  if (sem_id == -1) {
    perror("Errore nell'apertura del semaforo");
    return 1;
  }

  // Attende che il semaforo sia a 1
  struct sembuf op;
  op.sem_num = 0;
  op.sem_op = -1;
  op.sem_flg = 0;
  if (semop(sem_id, &op, 1) == -1) {
    perror("Errore nell'attesa del semaforo");
    return 1;
  }

  // Esegue il codice del processo atomo
  printf("Processo atomo in esecuzione...\n");
  // ...

  return 0;
}