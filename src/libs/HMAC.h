#include "gmp.h"

int generateKey_HMAC();
int hashing_HMAC(char* fileName, char* keyFileName);
int verify_HMAC(char* fileName, char* keyFileName);