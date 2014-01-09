

#define MAX_KEY_LEN     128
#define HEAD_HASHLETS   10000
#define KEY_VALUE_PAIRS 100000

// create a random ASCII key
void random_key(char *key) {
    // minimum key length is 10
    int i, len = 10 + rand() % (MAX_KEY_LEN-10);
    // create the key
    for(i = 0; i < len; i++)
        key[i] = '!' + rand() % 94;
    // terminate the string
    key[i] = 0;
}

