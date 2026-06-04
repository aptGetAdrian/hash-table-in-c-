// -*- C -*-
#ifndef _HASHTABLE_H
#define _HASHTABLE_H

#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#define INITIAL_CAPACITY 16

///////////////////////////
/// STRUCTURE DEFINITIONS
///////////////////////////

// hash table entry (slot may or may not be empty)
typedef struct {
    const char* key;
    void* value;
} item;

// hash table structure
typedef struct hashtable  {
    item* items;
    size_t capacity; // size of _entries array
    size_t length; // number of items in hash table
} ht;

// hash table iterator structure
typedef struct hti{
    const char* key; // current key
    void* value; // current value

    ht* _table; // reference to iterated hash table
    size_t _index; // current index into ht._entries
} hti;



///////////////////////////
/// FUNCTION DECLARATIONS
///////////////////////////

// creates a hash table and returns the pointer to it
ht* htCreate(void);

// takes a pointer to a hash table -> destorys it and frees allocated memory
void htDestroy(ht* table);

// get value of given key from the hash table -> return said value or NULL
void* htGet(ht* table, const char* key);

// set item with given key to given value (musn't be null)
const char* htSet(ht* table, const char* key, void* value);

// returns the number of items in hash table
size_t htLength(ht* table);

// hash table iterator functions

// return new hash table iterator (for htNext())
hti htIterator(ht* table);

// moves iterator to new item in hash table (updates iterator's key and value to current item)
bool htNext(hti* iterator);



///////////////////////////
/// FUNCTION DEFINITIONS
///////////////////////////

ht* htCreate(void) {
    ht* table = malloc(sizeof(ht));

    if (table == NULL) {
        // we could do separate error handling later or something idk
        return NULL;
    }

    table->length = 0;
    table->capacity = INITIAL_CAPACITY;

    table->items = calloc(table->capacity, sizeof(item));

    if (table->items == NULL) {
        free(table);
        return NULL;
    }

    return table;
}

void htDestroy(ht *table) {

    for (size_t i = 0; i < table->capacity; i++) {
        free((void*)table->items[i].key);
    }
}


#endif
