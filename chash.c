/*
 *
 * cHash v0.1 by Val Tenyotkin (val@tenyotk.in)
 *
 * See README.md for details.
 *
 */

#include <chash.h>

void cHash_fatal_error(char *message) {
    puts(message);
    exit(1);
}

// initialize the hash table
void cHash_init(cHash *hash, CHASH_INTEGER length) {

    // set the size
    hash->size = length;

    // allocate RAM
    if(!(hash->hashlets = (cHashlet *) malloc(length * sizeof(cHashlet))))
        cHash_fatal_error("Unable to allocate RAM for the hashlets.\n");

    // initialize the head hashlets
    while(length--) {
        #ifdef statistics
        hash->hashlets[length].list_length = 1;
        #endif
        hash->hashlets[length].next = CHASH_EMPTY;
    }

    // initialize the hashlet pool
    hash->pool_size  =
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
                    = (cHashlet *) malloc(CHASH_POOL * sizeof(cHashlet))))
            cHash_fatal_error("Unable to allocate additional RAM for the hashlet pool.\n");

        // reset the counters
        hash->pool_size  = CHASH_POOL;
        hash->pool_index = 0;
    }

    // return the pointer to an empty hashlet and increment the index
    return (struct cHashlet *)&hash->hashlet_pool[hash->pool_index++];
}

// compute the Jenkins hash of 'length' bytes located at *key
CHASH_CHASH cHash_hash(char *key, CHASH_INTEGER length) {

    CHASH_CHASH hash = 0;

    while(length--) {
        hash += key[length];
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

    #ifdef statistics
    // self-explanatory
    CHASH_INTEGER list_length = 0;
    cHashlet *head_hashlet = hashlet;
    #endif

    // if index is unused, initialize it
    if(hashlet->next == CHASH_EMPTY) {
        // store the key
        hashlet->key = hkey;
        // store the value
        hashlet->value = value;
        // end the hashlet list
        hashlet->next = CHASH_SET;

        return;
    }

    // if the index is used, loop through the linked list
    while(1) {

        #ifdef statistics
        // self-explanatory
        list_length++;
        #endif

        // if the key exactly matches an existing key, overwrite the value
        if(hashlet->key == hkey) {
            hashlet->value = value;
            return;
        }

        // stop when the end of the list has been reached
        if(hashlet->next == CHASH_SET)
            break;

        // proceed to the next hashlet in the linked list
        hashlet = (cHashlet *)hashlet->next;

    }

    /* new key-value pair */

    #ifdef statistics
    // update the list length and store it in the head hashlet
    head_hashlet->list_length = ++list_length;
    #endif

    // appropriate the RAM and attach the newly-created hashlet to the list
    hashlet = (cHashlet *)(hashlet->next = cHash_allocate_hashlet(hash));

    // store the key
    hashlet->key = hkey;
    // store the value
    hashlet->value = value;
    // set the .next to indicate the end of the hashlet list
    hashlet->next = CHASH_SET;

}
// get
int cHash_get(cHash *hash, char *key, long *value) {

    // hash the key
    CHASH_CHASH hkey = cHash_hash(key, strlen(key));

    // acquire the head hashlet
    cHashlet *hashlet = &hash->hashlets[hkey % hash->size];

    // if the hash is undefined, return false
    if(hashlet->next == CHASH_EMPTY)
        return CHASH_FALSE;

    // otherwise, travel down the list
    while(1) {

        // if the hash of the key matches, return the value
        if(hashlet->key == hkey) {
            *value = hashlet->value;
            return CHASH_TRUE;
        }

        // stop when the end of the list has been reached
        if(hashlet->next == CHASH_SET)
            break;

        // proceed to the next hashlet
        hashlet = (cHashlet *)hashlet->next;
    }

    // return false if the key doens't exist
    return CHASH_FALSE;
}
