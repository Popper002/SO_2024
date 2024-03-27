
#include "shared_memory.h"
int shmid;
struct  statistics *shared_data_in_shm;



void init_shared_memory(){


  shmid = shmget(STATISTICS_KEY,sizeof(struct statistics),IPC_CREAT | 0666);
  #ifdef _PRINT_DEBUG
    fprintf(stdout,"SHARED_MEMORY_ENERGY ID:%d\n",shmid);
  #endif
  if(shmid == -1){
    fprintf(stderr,"[%s]Error in shmget\n",__FILE__);
    exit(EXIT_FAILURE);
  }
/*  
  shared_data_in_shm = (struct statistics*) shmat(shmid,NULL,0);
  if(shared_data_in_shm == (struct statistics *)(-1)){
    fprintf(stderr,"[%s] Error in shmat\n",__FILE__);
    exit(EXIT_FAILURE);
    
    
  }
  */
}

void update_shared_memory(struct statistics *stats){
  memcpy(shared_data_in_shm,stats,sizeof(struct statistics)+1);
}
struct  statistics* read_shared_memory(){
  return shared_data_in_shm;
}

void cleanup_shared_memory(){
  shmdt(shared_data_in_shm);
  shmctl(shmid,IPC_RMID, NULL);
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