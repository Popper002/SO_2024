#include "hash_table.h"
#include <stdlib.h>
#include <string.h>
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

/**
 * garbage collect: coutn the elements in table, than check if resizing in
 * memory is needed using realloc, than check if the usage count is less than 0,
 * if so remove the element from the table
 */
void garbage_collect(struct hash_table *table)
{
  int total_elements = 0;

  for (int i = 0; i < table->max; i++)
  {
    struct elem_hash_table *entry = table->elements[i];
    while (entry != NULL)
    {
      total_elements++;
      entry = entry->next;
    }
  }

  if (total_elements < table->max / 2)
  {
    int new_capacity = total_elements * 2;
    struct elem_hash_table **new_alloc = realloc(table->elements, new_capacity);
    if (new_alloc != NULL)
    {
      table->elements = new_alloc;
      table->max = new_capacity;
    }
    else
    {
      fprintf(stderr, "Reallocation failed %s\n", strerror(errno));
      exit(EXIT_FAILURE);
    }
  }

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