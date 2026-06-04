// -*- C -*-
#ifndef _HASHTABLE_H
#define _HASHTABLE_H


#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#define INITIAL_CAPACITY 16

// these can be modified later
#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL


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
/// MAIN FUNCTION DECLARATIONS
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
/// SUPPORT FUNCTIONS DECLARATIONS
///////////////////////////

static uint64_t hashKey (const char* key);

static const char* htSerEntry(item* entries, size_t capacity,
        const char* key, void* value, size_t* plength);

static bool htExpand(ht* table);



///////////////////////////
/// MAIN FUNCTION DEFINITIONS
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
    // right now, the value assigned to each key can be any type, as the type for the value is void*
    // that means that the user has to manage their own allocated memory for the values.

    // TODO:
    // i am making it like this just for now in the beginning.
    // i'll rework it later and also add a custom destructor for the allocated memory for the values

    for (size_t i = 0; i < table->capacity; i++) {
        free((void*)table->items[i].key);
    }

    free(table->items);
    free(table);
}

void* htGet(ht* table, const char* key) {
    uint64_t hash = hashKey(key);

    // AND hash with capacity-1 to ensure it's within entries array
    size_t index = (size_t) (hash & (uint64_t)(table->capacity -1));

    // during insertion, we always place a key in the first empty slot after its hashed position
    // if we find NULL, that means the key way never inserted. that's why we can use the "!= NULL" condition
    while (table->items[index].key != NULL) {
        if (strcmp(key, table->items[index].key)) {

            return table->items[index].value;
        }

        index++;

        if (index >= table->capacity) {
            // wrapping around if we reach the end of entries
            index = 0;
        }

    }

    return NULL;

}



const char* htSet(ht *table, const char *key, void *value) {
     assert(value != NULL);

     if (value == NULL) {
         return NULL;
     }

     if (table->length >= (table->capacity / 2)) {
         if (!htExpand(table)) {
             return NULL;
         }
     }

     return htSerEntry(table->items, table->capacity, key, value, &table->length);

}


///////////////////////////
/// SUPPORT FUNCTIONS DEFINITIONS
///////////////////////////

static uint64_t hashKey (const char* key) {
    uint64_t hash = FNV_OFFSET;

    for (const char* p = key; *p != '\0'; p++) {

        // (unsigned char) - ensures no negative values
        // (uint64_t)- widens to 64 bits for XOR
        hash = hash ^ (uint64_t)(unsigned char) (*p);

        hash *= FNV_PRIME;
    }

    return hash;

}


#endif
