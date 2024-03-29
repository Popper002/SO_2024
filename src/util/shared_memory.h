#pragma once

#include "hash_table.h" // Include the header file for your hash table implementation

struct hash_table *attach_shared_memory();
void detach_shared_memory(struct hash_table *stats_map);
