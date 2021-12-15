#include "gmp.h"

#define MODE_STANDARD 0
#define MODE_CRT 1

#define NAME_FILE_PK_RSA "PK_standard_RSA.txt"
#define NAME_FILE_SK_RSA "SK_standard_RSA.txt"
#define NAME_FILE_K_DES "K_DES.txt"

#define NAME_FILE_CIPHER "cipher.txt"
#define NAME_FILE_PLAIN "message.txt"

#define NAME_FILE_SIGN "signature.txt"

#define IV "0x60400a3beaf63f57"

mpz_t z_n, z_e, z_d, z_keyDES, z_dp, z_dq, z_Ip, z_p, z_q;

int encryptFile_ECB(char* fileName, char* keyFileName);
int decryptFile_ECB(char* fileName, char* keyFileName);

int encryptFile_CBC(char* fileName, char* keyFileName);
int decryptFile_CBC(char* fileName, char* keyFileName);

int encryptFile_RSA(char* fileName, char* keyFileName, int mode);
int decryptFile_RSA(char* fileName, char* keyFileName, int mode);

int signFile_RSA(char* fileName, char* keyFileName, int mode);
int verifyFile_RSA(char *fileName, char *signfileName, char* keyFileName, int mode);

int generateFileKey_RSA();
int generateFileKey_RSA_CRT();
int generateFileKey_DES();