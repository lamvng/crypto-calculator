#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TAILLE_CLE 8

typedef unsigned char tab;
typedef tab cle[TAILLE_CLE];
typedef struct {
    tab *data;
    int len;
} String;

typedef tab sClefs[17][6]; /* 17 sets of 48 bits */


// This is just barely a minimum requirement, you can add more input or output if necessary

// Input: Nothing
// Output: A STRING (using string.h ) of 48 bit - Which is DES key
//char* generateKey_DES();
//tab generateKey_DES(void);

// Input: a 64-bit STRING of data, and 48-bit STRING of key
// Output: 64-bit string of encrypted data
String encrypt_DES(const cle key, const tab *message, int len);

// Input: a 64-bit STRING of data, and 48-bit STRING of key
// Output: 64-bit string of decrypted data
String decrypt_DES(const cle key, const tab *message, int len);
