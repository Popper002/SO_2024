#include "hash_table.h"
#include <stdlib.h>
#include <string.h>

/**
 * why +31 ?
 * https://stackoverflow.com/questions/299304/why-does-javas-hashcode-in-string-use-31-as-a-multiplier
 */
int hash_function(char *key, int max)
{
  int hash = 0;
  for (int i = 0; key[i] != '\0'; i++)
  {
    hash = (hash * 31 + key[i] % max);
  }
  printf("%s hash is: %d", __FILE__, hash);
  return hash;
}
void put(struct hash_table *table, char *key, int value)
{
  int index = hash_function(key, table->max);
  struct elem_hash_table *new_elem =
      (struct elem_hash_table *)malloc(sizeof(struct elem_hash_table));
  if (!new_elem)
  {
    return;
  }
  new_elem->key = strdup(key);
  new_elem->value = value;
  new_elem->next = table->elements[index];
  table->elements[index] = new_elem;
  table->number_of_elements++;
}

int get(struct hash_table *table, char *key)
{
  int index = hash_function(key, table->max);
  struct elem_hash_table *curr = table->elements[index];
  while (curr)
  {
    if (strcmp(curr->key, key) == 0)
    {
      return curr->value;
    }
    curr = curr->next;
  }
  return -1;
}

void remove_elem(struct hash_table *table, char *key)
{
  int index = hash_function(key, table->max);
  struct elem_hash_table *prev = NULL;
  struct elem_hash_table *curr = table->elements[index];
  while (curr)
  {
    if (strcmp(curr->key, key) == 0)
    {
      if (prev)
      {
	prev->next = curr->next;
      }
      else
      {
	table->elements[index] = curr->next;
      }
      free(curr->key);
      free(curr);
      table->number_of_elements--;
      return;
    }
    prev = curr;
    curr = curr->next;
  }
}
