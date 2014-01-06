#ifndef _CHASH_H
#define _CHASH_H

#define CHASH_BOOL      unsigned char
#define CHASH_TRUE      1
#define CHASH_FALSE     0
#define CHASH_INTEGER   long
#define CHASH_CHASH     unsigned long long
#define CHASH_EMPTY     (struct cHashlet *)0
#define CHASH_SET       (struct cHashlet *)1

#define CHASH_OCTET     8

#ifdef OSX
#include <stdlib.h>
#else
#include <malloc.h>
#endif

#include <string.h>

// unit of hash data storage
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

    // pointer to the array of hashlets
    cHashlet *hashlets;

    // length of the aforementioned array
    CHASH_INTEGER size;

} cHash;


// allocate memory for the hash
CHASH_BOOL cHash_init(cHash *hash, CHASH_INTEGER length) {

    CHASH_INTEGER i;

    // set the size
    hash->size = length;

    // allocate RAM
    if(!(hash->hashlets = (cHashlet *) malloc(length * sizeof(cHashlet))))
        // return false if RAM is lacking
        return CHASH_FALSE;

    // initialize the hashlets
    for(i = 0; i < length; i++)
        hash->hashlets[i].next = CHASH_EMPTY;

    return CHASH_TRUE;
}

// compute the hash of of 'length' bytes located at *key
CHASH_CHASH cHash_hash(char *key, CHASH_INTEGER length) {

    CHASH_INTEGER i;

    // initialize the hashing algorithm
    CHASH_CHASH hash  = 0xbee5e9b976d241ac;
    CHASH_CHASH octet = 0x9147e36a04ed7af0;

    // go through the key
    for(i = 0; i < length; i += CHASH_OCTET) {

        // take an octet, or less, from key
        memcpy(&octet, &key[i],
                length - i > CHASH_OCTET ? CHASH_OCTET : length - i);

        // hash it
        hash *= 0xd9afdeb758590198;
        hash *= hash + octet;
        hash ^= octet;
    }

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

        // proceed to the next hashlet
        hashlet = (cHashlet *)hashlet->next;

    }

    // new key-value pair

    // allocate the ram
    // DO TO: get the ram from the incrementally-reallocated pool
    if(!(hashlet->next = (struct cHashlet *) malloc(sizeof(cHashlet))))
        return;

    // point the hashlet to the newly-created hashlet
    hashlet = (cHashlet *)hashlet->next;

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
