# cHash
cHash is a hash table implementation in C.
## Use
### Code
```C
// example.c

#include <chash.h>

int main() {

    // declaration
    cHash products;

    // initialization with 137 buckets (a.k.a. head hashlets)
    cHash_init(&products, 137);

    // set
    cHash_set(&products, "Axe", 387);
    cHash_set(&products, "Box", 233);
    cHash_set(&products, "Hoe", 951);

    // get
    printf("%ld axes are in stock.\n", cHash_get(&products, "Axe"));

    return 0;
}
```
### Compilation
```bash
gcc -I. chash.c example.c -o example
```
If desired, `chash.c` can be precompiled.
### Testing
To compile the test suite:
```bash
make
```
To run the test suite:
```bash
cd test
./run
```
The test creates a hash table and injects it with random key-value pairs. Linked
list lengh statistics are computed.

## Technical Details
The hash table is implemented using separate chaining with linked lists.  The
table does not store the key, only the Jenkins hash of the key for hash
collision resolution.  Linked lists are allocated dynamically in increments of
`CHASH_POOL` hashlets (found in `chash.h`) once the RAM pool has been exhaused.
The amount of buckets (a.k.a. head hashlets) is static (137 in the example
above).

## Further Development
Dynamic resizing: if the average linked list exceeds, say, three hashlets,
double the amount of head hashlets (a.k.a. buckets).

