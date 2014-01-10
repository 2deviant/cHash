/*
 *
 * cHash v0.1 by Val Tenyotkin (val@tenyotk.in)
 *
 * See README.md for details.
 *
 */

#ifndef _CHASH_H
#define _CHASH_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#define CHASH_TRUE      1
#define CHASH_FALSE     0
#define CHASH_INTEGER   long
#define CHASH_CHASH     uint64_t
#define CHASH_EMPTY     (struct cHashlet *)0
#define CHASH_SET       (struct cHashlet *)1

// RAM is allocated in steps of this many hashlets:
#define CHASH_POOL      1024

// unit of hash table storage, hereafter as hashlet
typedef struct {

    // hash(key)
    CHASH_CHASH key;

    // value
    long value;

    // pointer to the next hashlet, if one exists
    struct cHashlet *next;

    #ifdef statistics
    CHASH_INTEGER list_length;
    #endif

} cHashlet;


// hash table structure
typedef struct {

    // pointer to the array of head hashlets
    cHashlet *hashlets;

    // length of the aforementioned array
    CHASH_INTEGER size;

    // pointer to the array of the hashlet pool
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

// allocate a hashlet for a linked list
struct cHashlet *cHash_allocate_hashlet(cHash *hash);

// compute the Jenkins hash of 'length' bytes located at *key
CHASH_CHASH cHash_hash(char *key, CHASH_INTEGER length);

// set
void cHash_set(cHash *hash, char *key, long value);

// get
int cHash_get(cHash *hash, char *key, long *value);

#endif
