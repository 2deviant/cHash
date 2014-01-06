#ifndef _CHASH_H
#define _CHASH_H

#define CHASH_BOOL      unsigned char
#define CHASH_TRUE      1
#define CHASH_FALSE     0
#define CHASH_INTEGER   long
#define CHASH_CHASH     unsigned long long
#define CHASH_EMPTY     (struct cHashlet *)0
#define CHASH_SET       (struct cHashlet *)1

#define KEY_LEN         32
#define OCTET           8

#ifdef OSX
#include <stdlib.h>
#else
#include <malloc.h>
#endif

#include <string.h>

// unit of hash data storage
typedef struct {

    // key sgtring
    char key[KEY_LEN];

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

    for(i = 0; i < length; i++)
        hash->hashlets[i].next = CHASH_EMPTY;

    return CHASH_TRUE;
}

CHASH_CHASH cHash_hash(char *key, CHASH_INTEGER length) {

    CHASH_INTEGER i;

    // initialize the hashing algorithm
    CHASH_CHASH hash  = 0xbee5e9b976d241ac;
    CHASH_CHASH octet = 0x9147e36a04ed7af0;

    // go through the key
    for(i = 0; i < length; i += OCTET) {

        // take an octet, or less, from key
        memcpy(&octet, &key[i], length - i > OCTET ? OCTET : length - i);

        // hash it!
        hash *= 0xd9afdeb758590198;
        hash *= hash + octet;
        hash ^= octet;
    }

    return hash;
}

// set
void cHash_set(cHash *hash, char *key, long value) {

    // compute the index of the *key
    CHASH_INTEGER index = cHash_hash(key, strlen(key)) % hash->size;
    
    // acquire the head hashlet
    cHashlet *link = &hash->hashlets[index];

    // if index is unused, initialize it
    if(link->next == CHASH_EMPTY) {
        #ifdef ebug
            printf("Unused index %ld.  %s = %ld\n", index, key, value);
        #endif
        // store the key
        strcpy(link->key, key);
        // store the value
        link->value = value;
        // set the .next to indicate the end of the hashlet chain
        link->next = CHASH_SET;
        // done
        return;
    }

    #ifdef ebug
        printf("Index collision. %s vs %s\n", link->key, key);
    #endif
    // if the index is used, loop through the chain
    while(1) {
        #ifdef ebug
            printf("... %s\n", link->key);
        #endif

        // if the key exactly matches an existing key, overwrite the value
        if(!strcmp(link->key, key)) {
            #ifdef ebug
                printf("Overwrite.  %s = %ld\n", key, value);
            #endif
            link->value = value;
            return;
        }

        // stop when the end of the chain has been reached
        if(link->next == CHASH_SET)
            break;

        // proceed to the next link
        link = (cHashlet *)link->next;

    }

    // new key-value pair
    #ifdef ebug
        printf("New entry in chain %ld. %s = %ld\n", index, key, value);
    #endif

    // allocate the ram
    // DO TO: get the ram from the incrementally-reallocated pool
    if(!(link->next = (struct cHashlet *) malloc(sizeof(cHashlet)))) {
        #ifdef ebug
            printf("Unable to allocate RAM for the new hashlet.");
        #endif
        return;
    }

    // point the link to the newly-created hashlet
    link = (cHashlet *)link->next;

    // store the key
    strcpy(link->key, key);
    // store the value
    link->value = value;
    // bookkeeping
    link->next = CHASH_SET;
}

// get
long cHash_get(cHash *hash, char *key) {

    // compute the index of the *key
    CHASH_INTEGER index = cHash_hash(key, strlen(key)) % hash->size;

    // acquire the head hashlet
    cHashlet *link = &hash->hashlets[index];

    // if the hash is undefined, return false
    if(link->next == CHASH_EMPTY) {
        #ifdef ebug
            printf("Key %s does not exist.\n", key);
        #endif
        return CHASH_FALSE;
    }

    #ifdef ebug
        printf("Searching for %s in chain %ld.\n", key, index);
    #endif
    // otherwise travel down the chain
    while(1) {
        #ifdef ebug
            printf("... currently on %s\n", link->key);
        #endif
        // if the key matches, return the value
        if(!strcmp(link->key, key))
            return link->value;

        // stop when the end of the chain has been reached
        if(link->next == CHASH_SET) {
            #ifdef ebug
                printf("... chain ends with %s\n", link->key);
            #endif
            break;
        }

        // proceed to the next link
        link = (cHashlet *)link->next;

    }

    // return false if the key doens't exist
    return CHASH_FALSE;
}

#endif
