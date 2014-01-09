# cHash
cHash is a C hash table implementation.


## Use
### Code
```C
#include <chash.h>

int main() {

    // declaration
    cHash products;

    // initialization with 137 primary buckets
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
make
```
The test creates a hash table and injects it with random key-value pairs. Linked
list lengh statistics are computed.

## Technical Details
The hash table is implemented using separate chaining with linked lists.  The
table does not store the key, only the Jenkins hash of the key for hash
collision resolution.  Linked lists are allocated dynamically in increments of
`CHASH_POOL` links found in `chash.h` at a time once the RAM pool has been
exhaused.

