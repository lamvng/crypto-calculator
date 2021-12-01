#include "../libs/EncryptionMode.h"
#include <stdio.h>

int encrypt_ECB(char* fileName, char* keyFileName)
{
    // TO DO
    return 0;
}
int decrypt_ECB(char* fileName, char* keyFileName)
{
    // TO DO
    return 0;
}

int encrypt_CBC(char* fileName, char* keyFileName)
{
    // TO DO
    return 0;
}

int decrypt_CBC(char* fileName, char* keyFileName)
{
    // TO DO
    return 0;
}

int encrypt_RSA(char* fileName, char* keyFileName, int mode)
{
    switch (mode)
    {
    case MODE_STANDARD:
        /* code */
        break;
    case MODE_CRT:
        /* code */
        break;
    default:
        return -1;
    }
    return 0;
}
int decrypt_RSA(char* fileName, char* keyFileName, int mode)
{
    switch (mode)
    {
    case MODE_STANDARD:
        /* code */
        break;
    case MODE_CRT:
        /* code */
        break;
    default:
        return -1;
    }
    return 0;
}

int sign_RSA(char* fileName, char* keyFileName, int mode)
{
    switch (mode)
    {
    case MODE_STANDARD:
        /* code */
        break;
    case MODE_CRT:
        /* code */
        break;
    default:
        return -1;
    }
    return 0;
}

int verify_RSA(char* fileName, char* keyFileName, int mode)
{
    switch (mode)
    {
    case MODE_STANDARD:
        /* code */
        break;
    case MODE_CRT:
        /* code */
        break;
    default:
        return -1;
    }
    return 0;
}