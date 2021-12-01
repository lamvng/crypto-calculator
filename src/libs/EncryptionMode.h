#include "gmp.h"

#define MODE_STANDARD 0
#define MODE_CRT 1

mpz_t z_n, z_e, z_c, z_d, z_keyDES;

int encrypt_ECB(char* fileName, char* keyFileName);
int decrypt_ECB(char* fileName, char* keyFileName);

int encrypt_CBC(char* fileName, char* keyFileName);
int decrypt_CBC(char* fileName, char* keyFileName);

int encrypt_RSA(char* fileName, char* keyFileName, int mode);
int decrypt_RSA(char* fileName, char* keyFileName, int mode);

int sign_RSA(char* fileName, char* keyFileName, int mode);
int verify_RSA(char* fileName, char* keyFileName, int mode);