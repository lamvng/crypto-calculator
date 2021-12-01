#include "gmp.h"

#define MODE_STANDARD 0
#define MODE_CRT 1

#define NAME_FILE_KEY_RSA "key_standard_RSA.txt"

mpz_t z_n, z_e, z_c, z_d, z_keyDES;

int encryptFile_ECB(char* fileName, char* keyFileName);
int decryptFile_ECB(char* fileName, char* keyFileName);

int encryptFile_CBC(char* fileName, char* keyFileName);
int decryptFile_CBC(char* fileName, char* keyFileName);

int encryptFile_RSA(char* fileName, char* keyFileName, int mode);
int decryptFile_RSA(char* fileName, char* keyFileName, int mode);

int signFile_RSA(char* fileName, char* keyFileName, int mode);
int verifyFile_RSA(char* fileName, char* keyFileName, int mode);

int generateFileKey_RSA();