#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

struct atom
{
  pid_t pid;
  int atomic_number;
  int atomic_flag;
  int reference_count;
};

struct config
{
  int N_ATOMI_INIT;
  int ENERGY_DEMAND;
  int N_ATOM_MAX;
  int MIN_A_ATOMICO;
  int N_NUOVI_ATOMI;
  int SIM_DURATION;
  int ENERGY_EXPLODE_THRESHOLD;
};

struct elem_hash_table
{
  int key; // TODO Change to pid_t for the fission project
  int value;
  int usage_count;
  struct elem_hash_table *next;
};

struct hash_table
{
  int max;
  int number_of_elements;
  struct elem_hash_table **elements;
  void (*put)(struct hash_table *table, int key, int value);
  int (*get)(struct hash_table *table, int key);
  void (*remove)(struct hash_table *table, int key);
  void (*decrement_usage_count)(struct hash_table *table);
  void (*garbage_collector)(struct hash_table *table);
};

int hash_function(int key, int max) { return key % max; }

void decrement_usage_count(struct hash_table *table)
{
  for (int i = 0; i < table->max; i++)
  {
    struct elem_hash_table *entry = table->elements[i];
    while (entry != NULL)
    {
      entry->usage_count--;
      entry = entry->next;
    }
  }
}

void put(struct hash_table *table, int key, int value)
{
  int index = hash_function(key, table->max);
  struct elem_hash_table *new_elem =
      (struct elem_hash_table *)malloc(sizeof(struct elem_hash_table));
  new_elem->key = key;
  new_elem->value = value;
  new_elem->next = table->elements[index];
  table->elements[index] = new_elem;
  table->number_of_elements++;
}

int get(struct hash_table *table, int key)
{
  int index = hash_function(key, table->max);
  struct elem_hash_table *entry = table->elements[index];
  while (entry != NULL)
  {
    if (entry->key == key)
    {
      entry->usage_count++;
      return entry->value;
    }
    entry = entry->next;
  }
  return -1;
}

void remove_elem(struct hash_table *table, int key)
{
  int index = hash_function(key, table->max);
  struct elem_hash_table *prev = NULL;
  struct elem_hash_table *current = table->elements[index];
  while (current != NULL)
  {
    if (current->key == key)
    {
      if (prev == NULL)
      {
	table->elements[index] = current->next;
      }
      else
      {
	prev->next = current->next;
      }
      free(current);
      table->number_of_elements--;
      return;
    }
    prev = current;
    current = current->next;
  }
}

void garbage_collect(struct hash_table *table)
{
  for (int i = 0; i < table->max; i++)
  {
    struct elem_hash_table *prev = NULL;
    struct elem_hash_table *current = table->elements[i];
    while (current != NULL)
    {
      struct elem_hash_table *next = current->next;
      if (current->usage_count <= 0)
      {
	if (prev == NULL)
	{
	  table->elements[i] = next;
	}
	else
	{
	  prev->next = next;
	}
	table->remove(table, current->key);
	free(current);
	table->number_of_elements--;
      }
      else
      {
	current->usage_count = 0;
      }
      prev = current;
      current = next;
    }
  }
}

void print_hash_table(struct hash_table *table)
{
  for (int i = 0; i < table->max; i++)
  {
    struct elem_hash_table *entry = table->elements[i];
    while (entry != NULL)
    {
      printf("key: %d value: %d usage count: %d\n", entry->key, entry->value,
	     entry->usage_count);
      entry = entry->next;
    }
  }
}

int main()
{
  srand(time(NULL));

  struct hash_table table;
  table.max = 100;
  table.number_of_elements = 0;
  table.elements = malloc(table.max * sizeof(struct elem_hash_table *));

  struct config config = {.N_ATOMI_INIT = 100,
              .ENERGY_DEMAND = 50,
              .N_ATOM_MAX = 10,
              .MIN_A_ATOMICO = 1,
              .N_NUOVI_ATOMI = 10,
              .SIM_DURATION = 500,
              .ENERGY_EXPLODE_THRESHOLD = 80};

  table.put = put;
  table.get = get;
  table.remove = remove_elem;
  table.decrement_usage_count = decrement_usage_count;
  table.garbage_collector = garbage_collect;

  for (int i = 0; i < config.N_ATOM_MAX; i++)
  {
    int random_key = rand() % 1000;
    int random_value = rand(); 
    table.put(&table, random_key, random_value);
  }

  printf("Before garbage collection:\n");
  print_hash_table(&table);

  // Decrement usage count of all elements to zero
  for (int i = 0; i < table.max; i++)
  {
    struct elem_hash_table *entry = table.elements[i];
    while (entry != NULL)
    {
      if(entry->value % 2 == 0){
        entry->usage_count = 1;
      }
      else{
        entry->usage_count = 0;
      }
      entry = entry->next;
    }
  }

  table.garbage_collector(&table);

  printf("After garbage collection:\n");
  print_hash_table(&table);

  return 0;
}

// int main()
// {
//   srand(time(NULL));

//   struct hash_table table;
//   table.max = 100;
//   table.number_of_elements = 0;
//   table.elements = malloc(table.max * sizeof(struct elem_hash_table *));

//   struct config config = {.N_ATOMI_INIT = 100,
// 			  .ENERGY_DEMAND = 50,
// 			  .N_ATOM_MAX = 10,
// 			  .MIN_A_ATOMICO = 1,
// 			  .N_NUOVI_ATOMI = 10,
// 			  .SIM_DURATION = 500,
// 			  .ENERGY_EXPLODE_THRESHOLD = 80};

//   table.put = put;
//   table.get = get;
//   table.remove = remove_elem;
//   table.decrement_usage_count = decrement_usage_count;
//   table.garbage_collector = garbage_collect;

//   for (int i = 0; i < config.N_ATOM_MAX; i++)
//   {
//     int random_key = rand() % 1000;
//     int random_value = rand(); 
//     table.put(&table, random_key, random_value);
//   }


//   print_hash_table(&table);

//   /* 
//       while (1) {
// 	  table.garbage_collector(&table);
// 	  table.decrement_usage_count(&table);
// 	  sleep(1);
//       }
//   */

//   return 0;
// }
