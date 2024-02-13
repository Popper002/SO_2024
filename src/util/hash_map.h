#pragma once
#include "../header/common.h"
struct elem_hash_table
{
  pid_t key;
  int value;
  int usage_count;
  struct elem_hash_table *next;
};

struct hash_table
{
  int max;
  int number_of_elements;
  struct elem_hash_table **elements;
  void (*put)(struct hash_table *table, pid_t key, int value);
  int (*get)(struct hash_table *table, int key);
  void (*remove)(struct hash_table *table, int key);
  void (*decrement_usage_count)(struct hash_table *table);
  void (*garbage_collector)(struct hash_table *table);
};

int hash_function(int key, int max);
void decrement_usage_count(struct hash_table *table);
void put(struct hash_table *table, int key, int value);
int get(struct hash_table *table, int key);
void remove_elem(struct hash_table *table, int key);
void garbage_collect(struct hash_table *table);
void print_hash_table(struct hash_table *table);