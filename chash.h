#ifndef _CHASH_H
#define _CHASH_H

#define KEY_LEN 32
#define CHASH_BOOL      unsigned char
#define CHASH_TRUE      1
#define CHASH_FALSE     0
#define CHASH_INTEGER   unsigned long

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

    hash->size = length;

    return CHASH_TRUE;
}

#endif
