#pragma once

#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STATISTICS_KEY 0x30
struct statistics
{
//  struct config conf;
  int total_num_activation_;
  int num_activation_last_sec;
  int num_fission_last_sec;
  int total_num_fission;
  int energy_produced_value;
  int total_num_energy_produced_last_sec;
  int total_num_energy_consumed;
  int num_energy_consumed_last_sec;
  int num_energy_consumed_inhibitor;
  int num_energy_consumed_inhibitor_last_sec;
  int total_nuclear_waste;
  int total_nuclear_waste_last_sec;
  int energy_absorbed;
  int fission_limited;
};



void init_shared_memory();
void update_shared_memory(struct statistics *stats);
struct  statistics* read_shared_memory();
void cleanup_shared_memory();


