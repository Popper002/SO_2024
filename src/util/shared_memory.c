
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
  shared_data_in_shm = (struct statistics*) shmat(shmid,NULL,0);
  if(shared_data_in_shm == (struct statistics *)(-1)){
    fprintf(stderr,"[%s] Error in shmat\n",__FILE__);
    exit(EXIT_FAILURE);
  }
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

