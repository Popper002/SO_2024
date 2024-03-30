#include "hash_table.h"
#include <stdlib.h>
#include <string.h>

/**
 * why << 5?
 * https://stackoverflow.com/questions/7666509/hash-function-for-string
 */
int hash_function(char *key, int max)
{
  int hash = 0;
  for (int i = 0; key[i] != '\0'; i++)
  {
    hash = (hash << 5)  + key[i] % max;
  }
  printf("%s hash is: %d", __FILE__, hash); return hash;
}

void put(struct hash_table *table, char *key, int value)
{
  printf("put: hash table max is %d",table->max);
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
  printf("get: hash table max is %d",table->max);
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

