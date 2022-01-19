#include "gmp.h"

/*Generate keySize bits key and save to keyFileName file
 * keySize: size (bit) of key
 * keyFileName: name of the file to save the key
 * Return: 0 if fail, 1 otherwise
 */
int generateKey_HMAC(int keySize, char* keyFileName);

/*hashing_HMAC: produce hmac for a given pair of (message, key)
 * fileName: message file
 * keyFileName: key file
 * hmacFileName: file name to save produced hmac
 * return 0 if hashing successes -1 otherwise
 */
int hashing_HMAC(char* fileName, char* keyFileName, char* hmacFileName);

/*Verify HMAC: given the message, key and correspond hmac, check if the hmac is HMAC(mess, key)
 * fileName: message file
 * keyFileName: key file
 * hmacFileName: hmac file to check if it's legitimate
 * return 0 if hashing successes -1 otherwise
 * */
int verify_HMAC(char* fileName, char* keyFileName, char* hmacFileName);