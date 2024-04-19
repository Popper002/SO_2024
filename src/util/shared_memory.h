#pragma once


struct hash_table *attach_shared_memory();
void detach_shared_memory(struct hash_table *stats_map);
