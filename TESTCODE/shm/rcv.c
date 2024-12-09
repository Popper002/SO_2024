#include "library.h"
    int id_shm; 
    key_t key;
    pid_t *shm_pid; 
static void fetch_argv(const * argv[])
{ 
    id_shm = atoi(argv[1]);
    key = atoi(argv[2]); 
    argv[3]= NULL; 
}
// Funzione per creare 3 processi figli e salvare i PID in un array
void crea_figli(pid_t *pids) {
    int i;

    // Crea 3 processi figli
    for (i = 0; i < 3; i++) {
        pids[i] = fork();

        // Controlla se la creazione del processo figlio ha avuto successo
        if (pids[i] < 0) {
            perror("Errore durante la creazione del processo figlio");
            exit(EXIT_FAILURE);
        }

        // Se è il processo figlio, termina
        if (pids[i] == 0) {
            exit(0);
        }
    }
}
int stampaStatoMemoria(int shid) {

  struct shmid_ds buf;

  if (shmctl(shid,IPC_STAT,&buf)==-1) {
    fprintf(stderr, "%s: %d. Errore in shmctl #%03d: %s\n", __FILE__, __LINE__, errno, strerror(errno));
    return -1;
  } else {
    printf("\nSTATISTICHE\n");
    printf("AreaId: %d\n",shid);
    printf("Dimensione: %ld\n",buf.shm_segsz);
    printf("Ultima shmat: %s\n",ctime(&buf.shm_atime));
    printf("Ultima shmdt: %s\n",ctime(&buf.shm_dtime));
    printf("Ultimo processo shmat/shmdt: %d\n",buf.shm_lpid);
    printf("Processi connessi: %ld\n",buf.shm_nattch);
    printf("\n");
    return 0;
  }
}
int main(int argc, char const *argv[])
{
    int i,j;
    i=0;
    printf("{%s} [%s] {PID %d}\n",__func__, __FILE__,getpid()); 
    fetch_argv(argv); 
    printf(" SHID %d  , KEY %d\n",id_shm , key );
    MyData *data; 
    pid_t pids[3]; // Array per memorizzare i PID dei figli

    // Crea i 3 processi figli
    crea_figli(pids);

    // Stampa i PID dei figli
    for (int i = 0; i < 3; i++) {
        printf("PID figlio %d: %d\n", i, pids[i]);
    }   
    data = (MyData*)shmat(id_shm,NULL,0);
    printf("AFTER ATTACH\n"); 
    memcpy(data->array, pids, sizeof(pids));   
    printf(" COPY COMPLEATE\n"); 
    stampaStatoMemoria(id_shm);
    return 0;
}
