#ifndef _HASHTABLE_H
#define _HASHTABLE_H

#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>

///////////////////////////
/// FUNCTION DECLARATIONS
///////////////////////////

typedef struct hash_table ht; // hash table structure

ht* htCreate(void); // creates a hash table and returns the pointer to it
void htDestroy(ht* table); // takes a pointer to a hash table -> destorys it and frees allocated memory
void* htGet(ht* table, const char* key); // get value of given key from the hash table -> return said value or NULL
const char* htSet(ht* table, const char* key, void* value); // set item with given key to given value (musn't be null)
size_t htLength(ht* table); // returns the number of items in hash table

// hash table iterator structure and functions
typedef struct hti hti;

hti htIterator(ht* table); // return new hash table iterator (for htNext())
bool htNext(hti* iterator); // moves iterator to new item in hash table (updates iterator's key and value to current item)



///////////////////////////
/// FUNCTION DEFINITIONS
///////////////////////////
struct hti{
    const char* key; // current key
    void* value; // current value

    ht* _table; // reference to iterated hash table
    size_t _index; // current index into ht._entries
};
#endif
