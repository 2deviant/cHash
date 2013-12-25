#include <stdio.h>
#include "chash.h"

int main() {

    cHash hash;

    cHash_init(&hash, 100);

    printf("%ld\n", hash.size);


    return 0;
}
