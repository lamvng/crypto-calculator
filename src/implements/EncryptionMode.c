#include "../libs/EncryptionMode.h"
#include <stdio.h>
#include "../libs/Module.h"
#include "../libs/RSA.h"

int encryptFile_ECB(char* fileName, char* keyFileName)
{
    // TO DO
    return 0;
}
int decryptFile_ECB(char* fileName, char* keyFileName)
{
    // TO DO
    return 0;
}

int encryptFile_CBC(char* fileName, char* keyFileName)
{
    // TO DO
    return 0;
}

int decryptFile_CBC(char* fileName, char* keyFileName)
{
    // TO DO
    return 0;
}

int encryptFile_RSA(char* fileName, char* keyFileName, int mode)
{
    switch (mode)
    {
    case MODE_STANDARD:
        // char* key = readFile(keyFileName);
        
        break;
    case MODE_CRT:
        /* code */
        break;
    default:
        return -1;
    }
    return 0;
}
int decryptFile_RSA(char* fileName, char* keyFileName, int mode)
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

int signFile_RSA(char* fileName, char* keyFileName, int mode)
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

int verifyFile_RSA(char* fileName, char* keyFileName, int mode)
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

int generateFileKey_RSA(){
    // get key for RSA z_e, z_d, z_n
    generateKey_RSA(z_e, z_d);

    // Save to file
    writeFileByLine(NAME_FILE_KEY_RSA, "abc");
}