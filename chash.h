/*
 * cHash v0.1 by Val Tenyotkin (val@tenyotk.in)
 *
 * cHash is an hash table implementation using separate chaining and linked
 * lists.  The table does not store the key, only the hash of the key.  
 *
 */

#ifndef _CHASH_H
#define _CHASH_H

#include <stdio.h>

#define CHASH_FALSE     0
#define CHASH_INTEGER   long
#define CHASH_CHASH     uint64_t
#define CHASH_EMPTY     (struct cHashlet *)0
#define CHASH_SET       (struct cHashlet *)1

/*
 * RAM is allocated in steps of this many hashlets:
 *
 */

#define CHASH_POOL      1024

/*
 * Darwin implementation of gcc keeps malloc() in stdlib.h, not malloc.h
 *
 */

#ifdef OSX
    #include <stdlib.h>
#else
    #include <malloc.h>
#endif

#include <string.h>

// unit of hash table storage
typedef struct {

    // hash(key)
    CHASH_CHASH key;

    // value
    long value;

    // pointer to the next hashlet, if one exists
    struct cHashlet *next;

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
void cHash_init(cHash *hash, CHASH_INTEGER length) {

    CHASH_INTEGER i;

    // set the size
    hash->size = length;

    // allocate RAM
    if(!(hash->hashlets = (cHashlet *) malloc(length * sizeof(cHashlet)))) {
        printf("Unable to allocate RAM for the hashlets.\n");
        exit(1);
    }

    // initialize the head hashlets
    for(i = 0; i < length; i++)
        hash->hashlets[i].next = CHASH_EMPTY;

    // initialize the hashlet pool
    hash->pool_size  = 0;
    hash->pool_index = 0;
}

// allocate a hashlet for a linked list, increase the size
// of the hashlet pool if necessary
struct cHashlet *cHash_allocate_hashlet(cHash *hash) {

    // if the hashlet pool has been exhausted (e.g. on first call)
    if(hash->pool_index == hash->pool_size) {
        // allocate more hashlets
        // NOTE: the original pointer to the hashlet pool is overwritten
        if(!(hash->hashlet_pool
                    = (cHashlet *) malloc(CHASH_POOL * sizeof(cHashlet)))) {
            printf("Unable to allocate additional RAM for the hashlet pool.\n");
            exit(1);
        }

        // reset the counters
        hash->pool_size  = CHASH_POOL;
        hash->pool_index = 0;
    }

    // return the pointer to an empty hashlet
    return (struct cHashlet *)&hash->hashlet_pool[hash->pool_index++];
}

// compute the Jenkins hash of 'length' bytes located at *key
CHASH_CHASH cHash_hash(char *key, CHASH_INTEGER length) {

    CHASH_INTEGER i;
    CHASH_CHASH hash;

    for(hash = i = 0; i < length; i++) {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6); 
    }   

    hash += (hash << 3); 
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
}

// set
void cHash_set(cHash *hash, char *key, long value) {

    // hash the key
    CHASH_CHASH hkey = cHash_hash(key, strlen(key));

    // acquire the head hashlet
    cHashlet *hashlet = &hash->hashlets[hkey % hash->size];

    // if index is unused, initialize it
    if(hashlet->next == CHASH_EMPTY) {
        // store the key
        hashlet->key = hkey;
        // store the value
        hashlet->value = value;
        // set the .next to indicate the end of the hashlet chain
        hashlet->next = CHASH_SET;
        // done
        return;
    }

    // if the index is used, loop through the chain
    while(1) {

        // if the key exactly matches an existing key, overwrite the value
        if(hashlet->key == hkey) {
            hashlet->value = value;
            return;
        }

        // stop when the end of the chain has been reached
        if(hashlet->next == CHASH_SET)
            break;

        // proceed to the next hashlet in the linked list
        hashlet = (cHashlet *)hashlet->next;

    }
            

    /* new key-value pair */

    // appropriate the RAM and attach the newly-created hashlet to the chain
    hashlet = (cHashlet *)(hashlet->next = cHash_allocate_hashlet(hash));

    // store the key
    hashlet->key = hkey;
    // store the value
    hashlet->value = value;
    // set the .next to indicate the end of the hashlet chain
    hashlet->next = CHASH_SET;

}

// get
long cHash_get(cHash *hash, char *key) {

    // hash the key
    CHASH_CHASH hkey = cHash_hash(key, strlen(key));

    // acquire the head hashlet
    cHashlet *hashlet = &hash->hashlets[hkey % hash->size];

    // if the hash is undefined, return false
    if(hashlet->next == CHASH_EMPTY)
        return CHASH_FALSE;

    // otherwise, travel down the chain
    while(1) {

        // if the hash of the key matches, return the value
        if(hashlet->key == hkey)
            return hashlet->value;

        // stop when the end of the chain has been reached
        if(hashlet->next == CHASH_SET)
            break;

        // proceed to the next hashlet
        hashlet = (cHashlet *)hashlet->next;
    }

    // return false if the key doens't exist
    return CHASH_FALSE;
}

#endif
