#include "gmp.h"

int generateKey_HMAC(int keySize, char* keyFileName);
int hashing_HMAC(char* fileName, char* keyFileName, char* hmacFileName);
int verify_HMAC(char* fileName, char* keyFileName, char* hmacFileName);