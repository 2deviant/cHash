#ifndef _CHASH_H
#define _CHASH_H

#define CHASH_BOOL      unsigned char
#define CHASH_TRUE      1
#define CHASH_FALSE     0
#define CHASH_INTEGER   long
#define CHASH_CHASH     unsigned long long
#define CHASH_EMPTY     0
#define CHASH_SET       1


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

    int i;

    // initialize the hashing algorithm
    CHASH_CHASH hash  = 0xbee5e9b976d241ac;
    CHASH_CHASH octet = 0x9147e36a04ed7af0;

    // go through the key
    for(i = 0; i < length; i += OCTET) {

        // take an octet from key
        memcpy(&octet, &key[i], i < length ? OCTET : i - length);

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
    int index = cHash_hash(key, strlen(key)) % hash->size;
    
    // if index is unused
    if(hash->hashlets[index].next == CHASH_EMPTY) {
        // store the key
        strcpy(hash->hashlets[index].key, key);
        // store the value
        hash->hashlets[index].value = value;
        // set the .next to indicate the end of the hashlet chain
        hash->hashlets[index].next = (struct cHashlet *)CHASH_SET;
    }

}

// get
long cHash_get(cHash *hash, char *key) {

    // compute the index of the *key
    int index = cHash_hash(key, strlen(key)) % hash->size;

    // if the hashlet is set and at the end of the chain
    if(hash->hashlets[index].next == (struct cHashlet *)CHASH_SET)
        // return the value
        return hash->hashlets[index].value;

    return CHASH_FALSE;
}

#endif
