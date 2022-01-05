#include "gmp.h"

int generateKey_HMAC(int key_size);
int hashing_HMAC(char* fileName, char* keyFileName, char* hmacFileName);
int verify_HMAC(char* fileName, char* keyFileName, char* hmacFileName);