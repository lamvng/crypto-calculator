#include "../libs/AES.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define ROUND_NUMBER 10
#define Nb 4
#define Nk 4

//AES 128 with 10 rounds
unsigned char SBOX_256[256] =
    {
        0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
        0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
        0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
        0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
        0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
        0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
        0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
        0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
        0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
        0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
        0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
        0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
        0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
        0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
        0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
        0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16};

unsigned char invSBOX_256[256] =
    {
        0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB,
        0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB,
        0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E,
        0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25,
        0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92,
        0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84,
        0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06,
        0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B,
        0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73,
        0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E,
        0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B,
        0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4,
        0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F,
        0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF,
        0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61,
        0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D};

char M[16] = {0x02, 0x03, 0x01, 0x01, 0x01, 0x02, 0x03, 0x01, 0x01, 0x01, 0x02, 0x03, 0x03, 0x01, 0x01, 0x02};

// TO DO auto generate invM from M;
char invM[16] = {0xd4, 0xa1, 0x07, 0x3b, 0x3b, 0xd4, 0xa1, 0x07, 0x07, 0x3b, 0xd4, 0xa1, 0xa1, 0x07, 0x3b, 0xd4};

char Rcon[11] = {0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};

void shiftRows(char *cipher)
{
    // shift row 2
    char temp = cipher[4];
    cipher[4] = cipher[5];
    cipher[5] = cipher[6];
    cipher[6] = cipher[7];
    cipher[7] = temp;

    // shift row 3
    temp = cipher[8];
    cipher[8] = cipher[10];
    cipher[10] = temp;
    temp = cipher[9];
    cipher[9] = cipher[11];
    cipher[11] = temp;

    // shift row 4
    temp = cipher[15];
    cipher[15] = cipher[14];
    cipher[14] = cipher[13];
    cipher[13] = cipher[12];
    cipher[12] = temp;
}

void inv_shiftRows(char *cipher)
{
    // shift row 2
    char temp = cipher[7];
    cipher[7] = cipher[6];
    cipher[6] = cipher[5];
    cipher[5] = cipher[4];
    cipher[4] = temp;

    // shift row 3
    temp = cipher[10];
    cipher[10] = cipher[8];
    cipher[8] = temp;
    temp = cipher[11];
    cipher[11] = cipher[9];
    cipher[9] = temp;

    // shift row 4
    temp = cipher[12];
    cipher[12] = cipher[13];
    cipher[13] = cipher[14];
    cipher[14] = cipher[15];
    cipher[15] = temp;
}

void mixColumns(char *cipher)
{
    int i;
    char cipherTemp[16];
    for (i = 0; i < 16; i++)
    {
        int j = i / 4;
        int count = 0;
        int temp = 0;
        for (count; count < 4; count++)
        {
            // printf("%d %d\n", (unsigned char) cipher[i % 4 + 4 * count], (unsigned char) M[j * 4 + count]);
            temp += (cipher[i % 4 + 4 * count] * M[j * 4 + count]);
        }
        temp %= 256;
        cipherTemp[i] = temp;
    }
    for (i = 0; i < 16; i++)
    {
        cipher[i] = cipherTemp[i];
    }
}

void inv_mixColumns(char *cipher)
{
    int i;
    char cipherTemp[16];
    for (i = 0; i < 16; i++)
    {
        int j = i / 4;
        int count = 0;
        int temp = 0;
        for (count; count < 4; count++)
        {
            // printf("%d %d\n", (unsigned char) cipher[i % 4 + 4 * count], (unsigned char) invM[j * 4 + count]);
            temp += (cipher[i % 4 + 4 * count] * invM[j * 4 + count]);
        }
        temp %= 256;
        cipherTemp[i] = temp;
    }
    for (i = 0; i < 16; i++)
    {
        cipher[i] = cipherTemp[i];
    }
}

int random_exact_k_bit(mpz_t n, int k, gmp_randstate_t state)
{
    mpz_t rop;
    mpz_init_set_ui(rop, 2);
    mpz_urandomb(n, state, k - 1);
    mpz_pow_ui(rop, rop, k - 1);

    mpz_add(n, n, rop);

    mpz_clear(rop);
    return 1;
}

// Input: Nothing
// Output: A STRING (using string.h ) of 128 bit - Which is AES key
void generateKey_AES(char *key)
{
    mpz_t z_genkey;
    mpz_inits(z_genkey , NULL);

    // random creation
    gmp_randstate_t generator;
    gmp_randinit_default(generator);
    int t = time(NULL);
    gmp_randseed_ui(generator,t);

    random_exact_k_bit(z_genkey, 128, generator);
    gmp_randclear(generator);
    char str[1000];
    mpz_get_str(str, 16, z_genkey);
    strcat(key, str);
    return;
}

void *processKey_AES(char *keyAES, char *subkeys[])
{
    int i;
    unsigned char round_key[Nb*(ROUND_NUMBER+1)*4];//Array of Nb*(ROUND_NUMBER+1) words for round key (each round takes Nb words)

    //First Nb words are the key itself
//    for (i = 0; i < Nk; ++i) {
//        round_key[i*4 + 0] =  (unsigned char) keyAES[i*4 + 0];
//        round_key[i*4 + 1] =  (unsigned char) keyAES[i*4 + 1];
//        round_key[i*4 + 2] =  (unsigned char) keyAES[i*4 + 2];
//        round_key[i*4 + 3] =  (unsigned char) keyAES[i*4 + 3];
//    }
    memcpy(round_key, keyAES, 16);

    unsigned char temp_word[4];
    unsigned char temp_char;
    for (i = Nk; i < Nb*(ROUND_NUMBER+1); i++)
    {
        // printf("i = %d\n", i);
        temp_word[0] = round_key[(i-1)*4 + 0];
        temp_word[1] = round_key[(i-1)*4 + 1];
        temp_word[2] = round_key[(i-1)*4 + 2];
        temp_word[3] = round_key[(i-1)*4 + 3];
        // printf("temp word: %02x%02x%02x%02x\n", temp_word[0], temp_word[1], temp_word[2], temp_word[3]);
        if (i%Nk == 0){
           //Rotword
            temp_char = temp_word[0];
            temp_word[0] = temp_word[1];
            temp_word[1] = temp_word[2];
            temp_word[2] = temp_word[3];
            temp_word[3] = temp_char;

            // printf("temp word after rotword: %02x%02x%02x%02x\n", temp_word[0], temp_word[1], temp_word[2], temp_word[3]);

            //Subword
            temp_word[0] = SBOX_256[temp_word[0]];
            temp_word[1] = SBOX_256[temp_word[1]];
            temp_word[2] = SBOX_256[temp_word[2]];
            temp_word[3] = SBOX_256[temp_word[3]];

            // printf("temp word after subword: %02x%02x%02x%02x\n", temp_word[0], temp_word[1], temp_word[2], temp_word[3]);
            //XOR Rcon
            temp_word[0] = temp_word[0]^Rcon[i/Nk];
            // printf("temp word after xor rcon: %02x%02x%02x%02x\n", temp_word[0], temp_word[1], temp_word[2], temp_word[3]);

        }
        //round_key[i] = round_key[i-Nk]^temp_word;
        round_key[i*4 + 0] = round_key[(i-Nk)*4 + 0]^temp_word[0];
        round_key[i*4 + 1] = round_key[(i-Nk)*4 + 1]^temp_word[1];
        round_key[i*4 + 2] = round_key[(i-Nk)*4 + 2]^temp_word[2];
        round_key[i*4 + 3] = round_key[(i-Nk)*4 + 3]^temp_word[3];

        // printf("Final word = %x%x%x%x\n", round_key[i*4 + 0], round_key[i*4 + 1], round_key[i*4 + 2], round_key[i*4 + 3]);
    }

    for (int j = 0; j < ROUND_NUMBER + 1; ++j) {
        subkeys[j] = (char*)malloc(sizeof(char)*Nk*4);
        memcpy(subkeys[j], round_key+(j*16), 16);
        // printf("Subkey round %d: ", j);
        // for (int k = 0; k < 16; ++k) {
        //     printf("%d", (unsigned char) subkeys[j][k]);
        // }
        // printf("\n");
    }

}

// Input: a 128-bit STRING of data, and 128-bit STRING of key
// Output: 128-bit string of encrypted data
char *encrypt_AES(char *plainText, char *keyAES, unsigned char *cipherText)
{
    int i, j;
    char *subkeys[ROUND_NUMBER];
    processKey_AES(keyAES, subkeys);

    int textSize = 16;

    // Round Key K_0
    for (j = 0; j < textSize; j++)
    {
        cipherText[j] = plainText[j] ^ subkeys[0][j];
    }

    for (i = 1; i < ROUND_NUMBER; i++)
    {
        // SubBytes
        // printf("ROUND %d: ", i);
        // for(j = 0; j < textSize; j++){
        //     printf(" %d", cipherText[j]);
        // }
        // printf("\n");
        for (j = 0; j < textSize; j++)
        {
            cipherText[j] = SBOX_256[cipherText[j]];
        }
        // ---------------

        // ShiftRows
        // printf("ROUND %d: ", i);
        // for(j = 0; j < textSize; j++){
        //     printf(" %d", cipherText[j]);
        // }
        // printf("\n");
        shiftRows(cipherText);
        // printf("ROUND %d: ", i);
        // for(j = 0; j < textSize; j++){
        //     printf(" %d", cipherText[j]);
        // }
        // printf("\n");
        // ---------------

        // mixColumns
        if (i != ROUND_NUMBER - 1)
        {
            mixColumns(cipherText);
        }
        // printf("ROUND %d: ", i);
        // for(j = 0; j < textSize; j++){
        //     printf(" %d", cipherText[j]);
        // }
        // printf("\n\n");
        // ---------------

        // AddRoundKey
        for (j = 0; j < textSize; j++)
        {
            cipherText[j] = cipherText[j] ^ subkeys[i][j];
        }
        // ---------------

        free(subkeys[i]);
    }

    return cipherText;
}

// Input: a 128-bit STRING of data, and 128-bit STRING of key
// Output: 128-bit string of decrypted data
char *decrypt_AES(unsigned char *cipherText, char *keyAES, char *plainText)
{
    int i, j;
    char *subkeys[ROUND_NUMBER];
    processKey_AES(keyAES, subkeys);

    int cipherSize = 16;

    for (i = ROUND_NUMBER-1; i > 0; i--)
    {
        // AddRoundKey
        for (j = 0; j < cipherSize; j++)
        {
            cipherText[j] = cipherText[j] ^ subkeys[i][j];
        }
        // ---------------

        // printf("ROUND %d: ", i);
        // for(j = 0; j < cipherSize; j++){
        //     printf(" %d", cipherText[j]);
        // }
        // printf("\n");
        if (i != ROUND_NUMBER - 1)
        {
            inv_mixColumns(cipherText);
        }

        // ShiftRows
        // printf("ROUND %d: ", i);
        // for(j = 0; j < cipherSize; j++){
        //     printf(" %d", cipherText[j]);
        // }
        // printf("\n");
        inv_shiftRows(cipherText);

        // printf("ROUND %d: ", i);
        // for(j = 0; j < cipherSize; j++){
        //     printf(" %d", cipherText[j]);
        // }
        // printf("\n");
        // ---------------

        // SubBytes
        for (j = 0; j < cipherSize; j++)
        {
            cipherText[j] = invSBOX_256[cipherText[j]];
        }
        // printf("ROUND %d: ", i);
        // for(j = 0; j < cipherSize; j++){
        //     printf(" %d", cipherText[j]);
        // }
        // printf("\n\n");
        // ---------------

        free(subkeys[i]);
    }

    // Round Key K_0
    for (j = 0; j < cipherSize; j++)
    {
        plainText[j] = cipherText[j] ^ subkeys[0][j];
    }

    return plainText;
}
