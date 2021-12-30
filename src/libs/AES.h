#include "gmp.h"

// This is just barely a minimum requirement, you can add more input or output if necessary

// Input: Nothing
// Output: A STRING (using string.h ) of 48 bit - Which is DES key
void generateKey_AES(char *key);

char *processKey_AES(char *bit_key);

// Input: a 64-bit STRING of data, and 48-bit STRING of key
// Output: 64-bit string of encrypted data
char* encrypt_AES(char *plainText, char *keyAES, char *cipherText);


// Input: a 64-bit STRING of data, and 48-bit STRING of key
// Output: 64-bit string of decrypted data
char* decrypt_AES(char *cipherText, char *keyAES, char *plainText);