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

#include <chash.h>

// initialize the hash
void cHash_init(cHash *hash, CHASH_INTEGER length) {

    CHASH_INTEGER i;

    // set the size
    hash->size = length;

    // allocate RAM
    if(!(hash->hashlets = (cHashlet *) malloc(length * sizeof(cHashlet)))) {
        fprintf(stderr, "Unable to allocate RAM for the hashlets.\n");
        exit(1);
    }

    // initialize the head hashlets
    for(i = 0; i < length; i++) {
        #ifdef statistics
        hash->hashlets[i].chain_length = 1;
        #endif
        hash->hashlets[i].next = CHASH_EMPTY;
    }

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
            fprintf(stderr, "Unable to allocate additional RAM for the hashlet pool.\n");
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

    #ifdef statistics
    // self-explanatory
    CHASH_INTEGER chain_length = 0;
    cHashlet *head_hashlet = hashlet;
    #endif

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

        #ifdef statistics
        // self-explanatory
        chain_length++;
        #endif

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

    #ifdef statistics
    // update the chain length and store it in the head hashlet
    head_hashlet->chain_length = ++chain_length;
    #endif

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
