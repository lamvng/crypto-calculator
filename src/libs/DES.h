#include <stdio.h>
#include <stdlib.h>
#include <string.h>



// Input: Nothing
// Output: char[64] ->64 bits
char* generateKey_DES();
//tab generateKey_DES(void);

// Input: message : char[64] 64-bits, and key: char[64] 64-bits
// Output: char[64] 64-bit encrypted data
char * encrypt(char * message, char * cle);

// Input: message : char[64] 64-bits, and key: char[64] 64-bits
// Output: String of 8 char
char * decrypt(char *message, char* cle);
