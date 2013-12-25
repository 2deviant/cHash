#ifndef _CHASH_H
#define _CHASH_H

#define KEY_LEN 32
#define NULL    0

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
    long size;

} cHash;


#endif
