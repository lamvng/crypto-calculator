#include "../libs/libBinChar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>


char* stringToBinaryWithLength(char* s, int size) {
    if(s == NULL) return 0; /* no input string */
    size_t len = size;
    char *binary = malloc(len*8 + 1); // each char is one byte (8 bits) and + 1 at the end for null terminator
    binary[0] = '\0';
    for(size_t i = 0; i < len; ++i) {
        char ch = s[i];
        for(int j = 7; j >= 0; --j){
            if(ch & (1 << j)) {
                strcat(binary,"1");
            } else {
                strcat(binary,"0");
            }
        }
    }
    return binary;
}

char* stringToBinary(char* s) {
    if(s == NULL) return 0; /* no input string */
    size_t len = strlen(s);
    char *binary = malloc(len*8 + 1); // each char is one byte (8 bits) and + 1 at the end for null terminator
    binary[0] = '\0';
    for(size_t i = 0; i < len; ++i) {
        char ch = s[i];
        for(int j = 7; j >= 0; --j){
            if(ch & (1 << j)) {
                strcat(binary,"1");
            } else {
                strcat(binary,"0");
            }
        }
    }
    return binary;
}
/*
*
*/
char* GetSubString(char* str, int index, int count) {
	int strLen = strlen(str);
	int lastIndex = index + count;

	if (index >= 0 && lastIndex > strLen) return "";

	char* subStr = malloc(count + 1);

	for (int i = 0; i < count; i++) {
		subStr[i] = str[index + i];
	}

	subStr[count] = '\0';

	return subStr;
}



/*
*
*/
char* AppendStringWithLength(const char* str1, const char* str2, int str1Len, int str2Len) {
	int strLen = str1Len + str2Len + 1;
	char* str = malloc(strLen);

	for (int i = 0; i < str1Len; i++)
		str[i] = str1[i];
	for (int i = 0; i < str2Len; i++)
		str[(str1Len + i)] = str2[i];
	str[strLen - 1] = '\0';
	return str;
}

char* AppendString(const char* str1, const char* str2) {
	int str1Len = strlen(str1);
	int str2Len = strlen(str2);
	int strLen = str1Len + str2Len + 1;
	char* str = malloc(strLen);

	for (int i = 0; i < str1Len; i++)
		str[i] = str1[i];
	for (int i = 0; i < str2Len; i++)
		str[(str1Len + i)] = str2[i];
	str[strLen - 1] = '\0';
	return str;
}

/*
*
*/
char* CharToString(char c) {
	char* str = malloc(2);
	str[0] = c;
	str[1] = '\0';

	return str;
}

/*
*
*/
int BinaryToDecimal(char* bin){
	int binLength = strlen(bin);
	double dec = 0;
	for (int i = 0; i < binLength; ++i)
	{
		dec += (bin[i] - 48) * pow(2, ((binLength - i) - 1));
	}
	return (int)dec;
}
/*
*
*/
char* BinaryToASCII(char* bin) {
	char* ascii = "";
	int binLen = strlen(bin);

	int asciiSize = 0;
	for (int i = 0; i < binLen; i += 8)
	{
		char* subStr = GetSubString(bin, i, 8);
		int dec = BinaryToDecimal(subStr);
		char* chr = CharToString(dec);
		ascii = AppendStringWithLength(ascii, chr, asciiSize, 1);
		free(subStr);
		free(chr);
		asciiSize++;
	}

	return ascii;
}
