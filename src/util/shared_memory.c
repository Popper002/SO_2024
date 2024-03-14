
#include "shared_memory.h"
int shmid;
struct energy_released *shared_data_in_shm;



void init_shared_memory(){


  shmid = shmget(ENERGY_KEY,sizeof(struct energy_released),IPC_CREAT | 0666);
  if(shmid == -1){
    fprintf(stderr,"[%s]Error in shmget\n",__FILE__);
    exit(EXIT_FAILURE);
  }
  shared_data_in_shm = (struct energy_released *) shmat(shmid,NULL,0);
  if(shared_data_in_shm == (struct energy_released *)(-1)){
    fprintf(stderr,"[%s] Error in shmat\n",__FILE__);
    exit(EXIT_FAILURE);
  }
}

void update_shared_memory(int energy){
  shared_data_in_shm->energy_released = energy;
}

int read_shared_memory(){
  return shared_data_in_shm->energy_released;
}

void cleanup_shared_memory(){
  shmdt(shared_data_in_shm);
  shmctl(shmid,IPC_RMID, NULL);
}

