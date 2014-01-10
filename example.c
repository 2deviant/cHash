#include <chash.h>

int main() {

    long count;

    // declaration
    cHash products;

    // initialization with 137 buckets (a.k.a. head hashlets)
    cHash_init(&products, 137);

    // set
    cHash_set(&products, "Axe", 387);
    cHash_set(&products, "Box", 233);
    cHash_set(&products, "Hoe", 951);

    // get
    cHash_get(&products, "Axe", &count);
    printf("%ld Axes are in stock.\n", count);

    // check if the key exists
    if(cHash_get(&products, "Fly", &count))
        printf("%ld Flies are in stock.\n", count);
    else
        printf("It appears that we do not carry Flies.\n");

    return 0;
}
