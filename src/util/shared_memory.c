
#include "shared_memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

#define KEY 0x200

struct hash_table *attach_shared_memory(){
  
  int shmid = shmget(KEY,sizeof(struct hash_table *) ,IPC_CREAT|0666);
  if(shmid == -1){
    fprintf(stderr,"Error in shmget shared memory %s\n",strerror(errno));
    exit(EXIT_FAILURE);
  }

  struct hash_table *stats_map = (struct hash_table *)shmat(shmid, NULL,0);
  if(stats_map == (void *) -1){
    fprintf(stderr,"Error attaching in shared memory\n");
    exit(EXIT_FAILURE);
  }

  return stats_map;

}

void detach_shared_memory(struct hash_table *stats_map){
  if(shmdt(stats_map) == -1){
    fprintf(stderr,"Error detaching in shared memory\n");
    exit(EXIT_FAILURE);
  }
  
}