

#define MAX_KEY_LEN     128
#define HEAD_HASHLETS   10007
#define KEY_VALUE_PAIRS 50000

// create a random ASCII key
void random_key(char *key) {
    // minimum key length is 10
    int i, len = 10 + rand() % (MAX_KEY_LEN-10);
    // create the key
    for(i = len; i-->0;)
        key[i] = '!' + rand() % 94;
    // terminate the string
    key[len] = 0;
}

