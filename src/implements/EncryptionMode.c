#include "../libs/EncryptionMode.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../libs/Module.h"
#include "../libs/RSA.h"
#include "../libs/DES.h"

void convertBytesToDecimal(mpz_t rop, unsigned char *buffer, int bytesPerBlock)
{
    int i;
    mpz_t pow_rs;
    mpz_init(pow_rs);
    mpz_set_d(rop, 0);
    for (i = 0; i < bytesPerBlock; i++)
    {
        mpz_ui_pow_ui(pow_rs, 256, bytesPerBlock - i - 1);
        mpz_addmul_ui(rop, pow_rs, buffer[i]);
    }

    mpz_clear(pow_rs);
}

void convertDecimalToBytes(mpz_t rop, unsigned char *buffer, int bytesPerBlock)
{
    int i;
    mpz_t quotient, remain;
    mpz_init_set(quotient, rop);
    mpz_init(remain);

    bytesPerBlock--;
    while (mpz_cmp_d(quotient, 0) > 0 || bytesPerBlock > -1)
    {
        mpz_fdiv_qr_ui(quotient, remain, quotient, 256);
        buffer[bytesPerBlock] = mpz_get_ui(remain);
        // gmp_printf("%Zd, %d\n", quotient, bytesPerBlock);
        bytesPerBlock--;
    }
    // gmp_printf("end\n");

    mpz_clears(quotient, remain, NULL);
}

int encryptFile_ECB(char *fileName, char *keyFileName)
{
    mpz_inits(z_keyDES, NULL);
    char *buffer;
    char str[250];

    // read key DES
    buffer = readFileByLine(keyFileName, 0);
    strncpy(str, buffer + 4, 254 - 4);
    mpz_set_str(z_keyDES, str, 0);
    free(buffer);

    // read data
    char *data = readFile(fileName);

    int blockSize = 8;

    int dataSize = strlen(data);
    int paddSize = blockSize - (dataSize % blockSize);
    char paddData[dataSize + paddSize + 1];

    strcpy(paddData, data);
    int i;
    for (i = paddSize; i > 0; i--)
    {
        paddData[dataSize + i - 1] = 0;
    }
    dataSize = dataSize + paddSize;
    paddData[dataSize] = '\0';

    char subData[blockSize];
    int count = 0, j = 0;
    mpz_t z_m, z_c;
    mpz_inits(z_m, z_c, NULL);
    char cipher[dataSize];
    cipher[dataSize] = '\0';

    for (i = 0; i < dataSize; i++)
    {
        subData[count] = paddData[i];
        count++;
        if (count == blockSize)
        {
            char bin[64];
            convertBytesToDecimal(z_m, subData, blockSize);
            mpz_get_str(bin, 2, z_m);

            // strcpy(bin, encrypt_DES());
            // TO DO

            mpz_set_str(z_c, bin, 2);
            // gmp_printf("%Zd, %Zd\n", z_m, z_c);
            convertDecimalToBytes(z_c, subData, blockSize);

            for (count--; count > -1; count--)
            {
                if (subData[count] == 0)
                {
                    subData[count] = 127;
                }
                cipher[j + count] = subData[count];
            }
            j += (blockSize);
            count = 0;
        }
    }

    writeNewFile(NAME_FILE_CIPHER, cipher);

    free(data);
    mpz_clears(z_keyDES, z_m, z_c, NULL);
    return 0;
}
int decryptFile_ECB(char *fileName, char *keyFileName)
{
    mpz_inits(z_keyDES, NULL);
    char *buffer;
    char str[250];

    // read key DES
    buffer = readFileByLine(keyFileName, 0);
    strncpy(str, buffer + 4, 254 - 4);
    mpz_set_str(z_keyDES, str, 0);
    free(buffer);

    // read cipher
    char *cipher = readFile(fileName);

    int blockSize = 8;

    int cipherSize = strlen(cipher);

    char subData[blockSize];
    int count = 0, j = 0, i;
    mpz_t z_m, z_c;
    mpz_inits(z_m, z_c, NULL);
    char data[cipherSize];
    data[cipherSize] = '\0';
    for (i = 0; i < cipherSize; i++)
    {
        if (cipher[i] == 127)
        {
            cipher[i] = 0;
        }
        subData[count] = cipher[i];
        count++;
        if (count == blockSize)
        {
            char bin[64];
            convertBytesToDecimal(z_c, subData, blockSize);
            mpz_get_str(bin, 2, z_c);

            // strcpy(bin, encrypt_DES());
            // TO DO

            mpz_set_str(z_m, bin, 2);
            convertDecimalToBytes(z_m, subData, blockSize);

            for (count--; count > -1; count--)
            {
                data[j + count] = subData[count];
            }
            j += (blockSize);
            count = 0;
        }
    }

    writeNewFile(NAME_FILE_PLAIN, data);

    free(cipher);
    mpz_clears(z_keyDES, z_m, z_c, NULL);
    return 0;
}

int encryptFile_CBC(char *fileName, char *keyFileName)
{
    // TO DO
    return 0;
}

int decryptFile_CBC(char *fileName, char *keyFileName)
{
    // TO DO
    return 0;
}

int encryptFile_RSA(char *fileName, char *keyFileName, int mode)
{
    switch (mode)
    {
    case MODE_STANDARD:
    {
        mpz_inits(z_n, z_e, NULL);
        char *buffer;
        char str[250];

        // read n
        buffer = readFileByLine(keyFileName, 0);
        strncpy(str, buffer + 4, 254 - 4);
        mpz_set_str(z_n, str, 0);
        free(buffer);

        // read e
        buffer = readFileByLine(keyFileName, 1);
        strncpy(str, buffer + 4, 254 - 4);
        mpz_set_str(z_e, str, 0);
        free(buffer);

        // read data
        char *data = readFile(fileName);

        // gmp_printf("%Zd, %Zd\n", z_n, z_e);

        mpz_get_str(str, 16, z_n);
        int blockSize = strlen(str) / 2 - 1;

        int dataSize = strlen(data);
        int paddSize = blockSize - (dataSize % blockSize);
        char paddData[dataSize + paddSize + 1];

        strcpy(paddData, data);
        int i;
        for (i = paddSize; i > 0; i--)
        {
            paddData[dataSize + i - 1] = 0;
        }
        dataSize = dataSize + paddSize;
        paddData[dataSize] = '\0';

        char subData[blockSize + 1];
        int count = 0, j = 0;
        mpz_t z_m, z_c;
        mpz_inits(z_m, z_c, NULL);
        int numberBlock = dataSize / blockSize;
        int encDataSize = (blockSize + 1) * numberBlock;

        char cipher[encDataSize];
        cipher[encDataSize] = '\0';
        for (i = 0; i < dataSize; i++)
        {
            subData[count] = paddData[i];
            count++;
            if (count == blockSize)
            {
                convertBytesToDecimal(z_m, subData, blockSize);
                encrypt_RSA(z_m, z_c, z_e, z_n);
                convertDecimalToBytes(z_c, subData, blockSize + 1);
                for (; count > -1; count--)
                {
                    if (subData[count] == 0)
                    {
                        subData[count] = 127;
                    }
                    cipher[j + count] = subData[count];
                }
                j += (blockSize + 1);
                count = 0;
            }
        }

        writeNewFile(NAME_FILE_CIPHER, cipher);

        free(data);
        mpz_clears(z_n, z_e, z_m, z_c, NULL);
        break;
    }
    case MODE_CRT:
    {
        encryptFile_RSA(fileName, keyFileName, MODE_STANDARD);
        break;
    }
    default:
        return -1;
    }
    return 0;
}
int decryptFile_RSA(char *fileName, char *keyFileName, int mode)
{
    switch (mode)
    {
    case MODE_STANDARD:
    {
        mpz_inits(z_n, z_d, NULL);
        char *buffer;
        char str[250];

        // read n
        buffer = readFileByLine(keyFileName, 0);
        strncpy(str, buffer + 4, 254 - 4);
        mpz_set_str(z_n, str, 0);
        free(buffer);

        // read d
        buffer = readFileByLine(keyFileName, 1);
        strncpy(str, buffer + 4, 254 - 4);
        mpz_set_str(z_d, str, 0);
        free(buffer);

        // read cipher
        char *cipher = readFile(fileName);

        mpz_get_str(str, 16, z_n);
        int blockSize = strlen(str) / 2;

        int cipherSize = strlen(cipher);
        // printf("%s\n", cipher);

        char subCipher[blockSize];
        int count = 0, j = 0, i;
        mpz_t z_m, z_c;
        mpz_inits(z_m, z_c, NULL);
        int numberBlock = cipherSize / blockSize;
        int dataSize = (blockSize - 1) * numberBlock;
        char data[dataSize];
        for (i = 0; i < cipherSize; i++)
        {
            if (cipher[i] == 127)
            {
                cipher[i] = 0;
            }
            subCipher[count] = cipher[i];

            count++;
            if (count == blockSize)
            {
                convertBytesToDecimal(z_c, subCipher, blockSize);
                decrypt_RSA(z_m, z_c, z_d, z_n);
                convertDecimalToBytes(z_m, subCipher, blockSize - 1);

                for (count--; count > -1; count--)
                {
                    data[j + count] = subCipher[count];
                }
                j += (blockSize - 1);
                count = 0;
            }
        }

        writeNewFile(NAME_FILE_PLAIN, data);

        free(cipher);
        mpz_clears(z_n, z_d, z_m, z_c, NULL);
        break;
    }
    case MODE_CRT:
    {
        mpz_inits(z_n, z_d, NULL);
        char *buffer;
        char str[250];

        // read dp
        buffer = readFileByLine(keyFileName, 1);
        strncpy(str, buffer + 4, 254 - 4);
        mpz_set_str(z_dp, str, 0);
        free(buffer);

        // read dq
        buffer = readFileByLine(keyFileName, 2);
        strncpy(str, buffer + 4, 254 - 4);
        mpz_set_str(z_dq, str, 0);
        free(buffer);

        // read Ip
        buffer = readFileByLine(keyFileName, 3);
        strncpy(str, buffer + 4, 254 - 4);
        mpz_set_str(z_Ip, str, 0);
        free(buffer);

        // read p
        buffer = readFileByLine(keyFileName, 4);
        strncpy(str, buffer + 4, 254 - 4);
        mpz_set_str(z_p, str, 0);
        free(buffer);

        // read q
        buffer = readFileByLine(keyFileName, 5);
        strncpy(str, buffer + 4, 254 - 4);
        mpz_set_str(z_q, str, 0);
        free(buffer);

        // read cipher
        char *cipher = readFile(fileName);

        mpz_get_str(str, 16, z_n);
        int blockSize = strlen(str) / 2;

        int cipherSize = strlen(cipher);
        // printf("%s\n", cipher);

        char subCipher[blockSize];
        int count = 0, j = 0, i;
        mpz_t z_m, z_c;
        mpz_inits(z_m, z_c, NULL);
        int numberBlock = cipherSize / blockSize;
        int dataSize = (blockSize - 1) * numberBlock;
        char data[dataSize];
        for (i = 0; i < cipherSize; i++)
        {
            if (cipher[i] == 127)
            {
                cipher[i] = 0;
            }
            subCipher[count] = cipher[i];

            count++;
            if (count == blockSize)
            {
                convertBytesToDecimal(z_c, subCipher, blockSize);
                
                // decrypt_RSA(z_m, z_c, z_d, z_n);
                // TO DO RSA_CRT

                convertDecimalToBytes(z_m, subCipher, blockSize - 1);

                for (count--; count > -1; count--)
                {
                    data[j + count] = subCipher[count];
                }
                j += (blockSize - 1);
                count = 0;
            }
        }

        writeNewFile(NAME_FILE_PLAIN, data);

        free(cipher);
        mpz_clears(z_dp, z_dq, z_p, z_q, z_Ip, z_m, z_c, NULL);
        break;
    }
    default:
        return -1;
    }
    return 0;
}

int signFile_RSA(char *fileName, char *keyFileName, int mode)
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

int verifyFile_RSA(char *fileName, char *keyFileName, int mode)
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

int generateFileKey_RSA()
{
    mpz_inits(z_n, z_e, z_d, NULL);

    // get key for RSA z_e, z_d, z_n
    mpz_set_d(z_n, 1000000);
    mpz_set_d(z_e, 200);
    mpz_set_d(z_d, 300);
    generateKey_RSA(z_e, z_d);

    // Save to file
    char key[1000];
    char str[1000] = "n = 0x";
    mpz_get_str(key, 16, z_n);
    strcat(str, key);
    strcat(str, "\n");
    writeNewFile(NAME_FILE_PK_RSA, str);
    writeNewFile(NAME_FILE_SK_RSA, str);

    strcpy(str, "e = 0x");
    mpz_get_str(key, 16, z_e);
    strcat(str, key);
    writeFileByLine(NAME_FILE_PK_RSA, str);

    strcpy(str, "d = 0x");
    mpz_get_str(key, 16, z_d);
    strcat(str, key);
    writeFileByLine(NAME_FILE_SK_RSA, str);

    mpz_clears(z_n, z_e, z_d, NULL);
}