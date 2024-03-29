
#include "shared_memory.h"
#include "hash_table.h"
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>


struct hash_table *attach_shared_memory(){
  key_t key = ftok("./bin/master",65);
  if(key == -1){
    fprintf(stderr,"Erro in ftok shared memory");
    exit(EXIT_FAILURE);
  }
  int shmid = shmget(key,sizeof(struct hash_table),0666);
  if(shmid == -1){
    fprintf(stderr,"Erro in shmget shared memory");
    exit(EXIT_FAILURE);
  }

  struct hash_table *stats_map = (struct hash_table *)shmat(shmid, NULL,0);
  if(stats_map == (void *) -1){
    fprintf(stderr,"Error attaching in shared memory");
    exit(EXIT_FAILURE);
  }

  return stats_map;

}

void detach_shared_memory(struct hash_table *stats_map){
  if(shmdt(stats_map) == -1){
    fprintf(stderr,"Error detaching in shared memory");
    exit(EXIT_FAILURE);
  }
}