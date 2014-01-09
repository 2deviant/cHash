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
