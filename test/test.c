#include <stdio.h>
#include <stdlib.h>
#include <chash.h>
#include <time.h>
#include "test.h"

int main() {

    /**************************************************************************/
    /*** Initialization *******************************************************/
    /**************************************************************************/

    // variable declarations
    unsigned seed = time(0);
    char key[MAX_KEY_LEN];
    long value, expected;
    cHash hash;
    int i;

    // notificaiton
    printf("Allocating a hash table with %ld head hashlets...\n",
            (long)HEAD_HASHLETS);

    // initialize hash
    cHash_init(&hash, HEAD_HASHLETS );


    /**************************************************************************/
    /*** Dump a lot of random key-value pairs into the hash table *************/
    /**************************************************************************/

    // notification
    printf("Inserting %ld random key-value pairs...\n", (long)KEY_VALUE_PAIRS);

    // explicit seed to insure reproducibility of the pseudo-random sequence
    srand(seed);

    // self-explanatory
    for(i = 0; i < KEY_VALUE_PAIRS; i++) {
        random_key(key);
        cHash_set(&hash, key, rand());
    }

    /**************************************************************************/
    /*** Extract the key-value pairs from above *******************************/
    /**************************************************************************/

    // notification
    printf("Extracting %ld key-value pairs from above...\n",
            (long)KEY_VALUE_PAIRS);

    // explicit seed to insure reproducibility of the pseudo-random sequence
    srand(seed);

    // self-explanatory
    for(i = 0; i < KEY_VALUE_PAIRS; i++) {
        random_key(key);
        expected = rand();
        value = cHash_get(&hash, key);
        if(expected != value)
            printf("Error: %s should be %ld but, is %ld instead\n",
                    key, expected, value);
    }

    /**************************************************************************/
    /*** Attempt to extract keys that should not exist ************************/
    /**************************************************************************/

    // notification
    printf("Attempting to extract %ld keys which do not exist...\n",
            (long)KEY_VALUE_PAIRS);

    // self-explanatory
    for(i = 0; i < KEY_VALUE_PAIRS; i++) {
        // the probability of a repeated ten-digit sequence is vanishingly small
        // however, a space is added just to be 100% sure
        random_key(key); key[1] = ' ';
        if(cHash_get(&hash, key))
            printf("Error.  This key should not exist: %s\n", key);
    }

    /**************************************************************************/
    /*** Attempt to extract keys that should not exist ************************/
    /**************************************************************************/

    // loop through head hashlets
    for(i=0; i<hash.size; i++)
        // if the hashlet is utilized 
        if(hash.hashlets[i].next)
            // print the lenght of the linked list
            fprintf(stderr, "%ld\n", hash.hashlets[i].list_length);

    return 0;
}
