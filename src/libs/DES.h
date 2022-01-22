#include <stdio.h>
#include <stdlib.h>
#include <string.h>




// Input: Nothing
// Output: char[64] ->64 bits
char* generateKey_DES();
//tab generateKey_DES(void);

// Input: message : 64-bits, and key: 64-bits
// Output: 64-bit encrypted data
char * encrypt(char * message, char * cle);

// Input: message : 64-bits, and key: 64-bits
// Output: 64-bit decrypted data
char * decrypt(char *message, char* cle);
