#include "gmp.h"

// This is just barely a minimum requirement, you can add more input or output if necessary

// Input: Nothing
// Output: A STRING (using string.h ) of 128 bit - Which is AES key
void generateKey_AES(char *key);

char *processKey_AES(char *bit_key, char *subkeys[]);

// Input: a 128-bit STRING of data, and 128-bit STRING of key
// Output: 128-bit string of encrypted data
char* encrypt_AES(char *plainText, char *keyAES,unsigned char *cipherText);


// Input: a 128-bit STRING of data, and 128-bit STRING of key
// Output: 128-bit string of decrypted data
char *decrypt_AES(unsigned char *cipherText, char *keyAES, char *plainText);