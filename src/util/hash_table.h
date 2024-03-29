#pragma once
#include "../header/common.h"
#include <string.h>

struct elem_hash_table
{
    char *key;
    int value;
    struct elem_hash_table *next;
};

struct hash_table
{
    int max;
    int number_of_elements;
    struct elem_hash_table **elements;
    void (*put)(struct hash_table *table, char *key, int value);
    int (*get)(struct hash_table *table, char *key);
    void (*remove)(struct hash_table *table, char *key);
    void (*garbage_collector)(struct hash_table *table);
};

int hash_function(const char *key, int max);
void put(struct hash_table *table, char *key, int value);
int get(struct hash_table *table, char *key);
void remove_elem(struct hash_table *table, char *key);
void garbage_collect(struct hash_table *table);
void print_hash_table(struct hash_table *table);