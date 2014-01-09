/*
 *
 * cHash v0.1 by Val Tenyotkin (val@tenyotk.in)
 *
 * A hash table implementation using separate chaining with linked lists.  The
 * table does not store the key, only the Jenkins hash of the key for hash
 * collision resolution. Linked lists are allocated dynamically in increments of
 * CHASH_POOL links (also hashlets) at a time.
 *
 */

#ifndef _CHASH_H
#define _CHASH_H

#include <stdio.h>
#include <string.h>

// Darwin implementation of gcc/clang keeps malloc() in stdlib.h, not malloc.h
#ifdef OSX
    #include <stdlib.h>
#else
    #include <malloc.h>
#endif

#define CHASH_FALSE     0
#define CHASH_INTEGER   long
#define CHASH_CHASH     uint64_t
#define CHASH_EMPTY     (struct cHashlet *)0
#define CHASH_SET       (struct cHashlet *)1

// RAM is allocated in steps of this many hashlets:
#define CHASH_POOL      1024

// unit of hash table storage
typedef struct {

    // hash(key)
    CHASH_CHASH key;

    // value
    long value;

    // pointer to the next hashlet, if one exists
    struct cHashlet *next;

    #ifdef statistics
    CHASH_INTEGER chain_length;
    #endif

} cHashlet;


// hash structure
typedef struct {

    // pointer to the array of head hashlets
    cHashlet *hashlets;

    // length of the aforementioned array
    CHASH_INTEGER size;

    // pointer to the array of the hashlet chain pool
    cHashlet *hashlet_pool;
    // the purpose of the pool is to allocate the RAM in bulk in order to avoid
    // repetitive malloc() calls for every index collision

    // current hashlet pool size
    CHASH_INTEGER pool_size;

    // current hashlet pool index
    CHASH_INTEGER pool_index;
} cHash;


// initialize the hash
void cHash_init(cHash *hash, CHASH_INTEGER length);

// allocate a hashlet for a linked list, increase the size
// of the hashlet pool if necessary
struct cHashlet *cHash_allocate_hashlet(cHash *hash);

// compute the Jenkins hash of 'length' bytes located at *key
CHASH_CHASH cHash_hash(char *key, CHASH_INTEGER length);

// set
void cHash_set(cHash *hash, char *key, long value);

// get
long cHash_get(cHash *hash, char *key);

#endif
