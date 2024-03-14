#pragma once

#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>


#define ENERGY_KEY 0x30
struct energy_released {
  int energy_released;
};



void init_shared_memory();
void update_shared_memory();
int read_shared_memory();
void cleanup_shared_memory();


