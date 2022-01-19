#include "../libs/HMAC.h"
#include "../libs/md5.h"
#include "gmp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#define char_to_hex(c) ( (toupper((c)) >= 'A' && toupper((c)) <= 'F')? toupper((c)) - 'A' + 10 : (c) - '0')

#define HASH_SIZE 16


int generateKey_HMAC(int keySize, char* keyFileName){
    /*Generate keySize bits key and save to keyFileName file
     * keySize: size (bit) of key
     * keyFileName: name of the file to save the key
     * Return: 0 if fail, 1 otherwise
     */
    gmp_randstate_t r_gen;
    gmp_randinit_default(r_gen);
    gmp_randseed_ui(r_gen, time(NULL));
    mpz_t key;
    mpz_init(key);
    while (mpz_cmp_ui(key, 0) == 0){
        mpz_urandomb(key, r_gen, keySize);
    }
    FILE *fp;
    fp = fopen(keyFileName, "w+");
    if(fp == NULL){
        perror("Failed: ");
        return 0;
    }
    mpz_out_str(fp, 16, key);
    fclose(fp);
    mpz_clear(key);
    gmp_randclear(r_gen);
    return 1;
}

unsigned char* read_file(FILE *fp, long file_size){
    /*Read file in 2 mode: hex file with 0x prefix or others types of file*/
    char prefix[3];
    unsigned char *file_content;
    fgets(prefix, 3, fp);
    if (!strcmp(prefix, "0x")){
        unsigned char c1, c2, c;
        file_size = file_size/2 - 1;
        file_content = malloc(file_size*sizeof(char));

        for (int i = 0; i < file_size; ++i) {
            c1 = fgetc(fp);
            c2 = fgetc(fp);
            c = 16*char_to_hex(c1) + char_to_hex(c2);
            file_content[i] = c;
        }


    } else{

        fseek(fp, 0, SEEK_SET);
        file_content = malloc(file_size*sizeof(char));
        fread(file_content, 1, file_size, fp);
    }
    return file_content;
}

int hashing_HMAC(char* fileName, char* keyFileName, char* hmacFileName){
    /*hashing_HMAC: produce hmac for a given pair of (message, key)
     * fileName: message file
     * keyFileName: key file
     * hmacFileName: file name to save produced hmac
     * return 0 if hashing successes -1 otherwise
     */

    FILE *fp;
    //Read key
    fp = fopen(keyFileName, "rb");
    if(fp == NULL){
        perror("Failed: ");
        return -1;
    }
    fseek(fp, 0, SEEK_END);
    long key_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);


    unsigned char *hmac_key;
    hmac_key = read_file(fp, key_size);

    fclose(fp);
    //Read message

    fp = fopen(fileName, "rb");
    if(fp == NULL){
        perror("Failed: ");
        return -1;
    }
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    unsigned char *file_content;
    file_content = read_file(fp, file_size);

    fclose(fp);

    //XOR key with ipad
    unsigned char k_ipad[65];
    unsigned char k_opad[65];

    unsigned char *tkey;

    //Hash key if len key > 64
    if(key_size > 64){
        printf("Key size exceeded, so hash\n");
        tkey = hashmd5(hmac_key);
        hmac_key = tkey;
        key_size = 16;
    }

    //XOR key with ipad, opad
    memset(k_ipad, 0, sizeof(k_ipad));
    memset(k_opad, 0, sizeof(k_opad) );
    memcpy(k_ipad,hmac_key, key_size);
    memcpy(k_opad,hmac_key,  key_size);

    for (int j = 0; j < 64; ++j) {
        k_ipad[j] ^= 0x36;
        k_opad[j] ^= 0x5c;
    }

    //Inner MD5: MD5(K_ipad || text)
    unsigned char *inner_digest;

    //Concat k_ipad & file_content
    unsigned char inner_content[file_size+64];
    memcpy(inner_content, k_ipad, 64);
    memcpy(inner_content+64, file_content, file_size);
    inner_digest = hashmd5(inner_content);


    //Outer MD5: MD5(k_opad||innerMD5)
    unsigned char *outer_digest;

    //Concat k_opad & innerMD5
    unsigned char outer_content[HASH_SIZE+64];
    memcpy(outer_content, k_opad, 64);
    memcpy(outer_content+64, inner_digest, HASH_SIZE);

    printf("==========================================\n");
    printf("Inner Digest: ");
    for (int i = 0; i < HASH_SIZE; ++i) {
        printf("%x", inner_digest[i]);
    }
    printf("\n");

    printf("k_opad: ");
    for (int i = 0; i < 64; ++i) {
        printf("%x", k_opad[i]);
    }

    printf("\n");
    printf("outer content: ");
    for (int i = 0; i < HASH_SIZE+64; ++i) {
        printf("%x", outer_content[i]);
    }
    printf("\n");

//    outer_digest = hashmd5(outer_content);
    printf("Digest: ");
    for (int i = 0; i < HASH_SIZE; ++i) {
        printf("%x", outer_digest[i]);
    }

    printf("\n");


    //Write hmac to file
    fp = fopen(hmacFileName, "w+");
    if(fp == NULL){
        perror("Failed: ");
        return -1;
    }

    fputs((char*)outer_digest, fp);
    fclose(fp);

    return 1;
}

int verify_HMAC(char* fileName, char* keyFileName, char* hmacFileName){
    /*Verify HMAC: given the message, key and correspond hmac, check if the hmac is HMAC(mess, key)
     * fileName: message file
     * keyFileName: key file
     * hmacFileName: hmac file to check if it's legitimate
     * return 0 if hashing successes -1 otherwise
     * */

    //Compute hmac for mess and key
    char* hmac_file;
    int len = strlen(hmacFileName);
    hmac_file = malloc((len+6)*sizeof(char));

    //Save new hmac
    strcpy(hmac_file, hmacFileName);
    strcat(hmac_file, "_temp");
    if(hashing_HMAC(fileName, keyFileName, hmac_file) == -1){
        printf("Hashing failed!\n");
        return -1;
    }


    FILE *fp;
    //Read input hmac file
    fp = fopen(hmacFileName, "r");
    if(fp == NULL){
        perror("Failed: ");
        return -1;
    }
    fseek(fp, 0, SEEK_END);
    long hmac_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char* hmac;
    hmac = malloc(hmac_size*sizeof(char));
    fgets(hmac, hmac_size, fp);

//    Read new gen hmac file
    fp = fopen(hmac_file, "r");
    if(fp == NULL){
        perror("Failed: ");
        return -1;
    }
    fseek(fp, 0, SEEK_END);
    long hmac_size2 = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char* hmac2;
    hmac2 = malloc(hmac_size*sizeof(char));
    fgets(hmac2, hmac_size2, fp);
    fclose(fp);
    remove(hmac_file);

    if(!strcmp(hmac, hmac2)){
        printf("abc");
        return 1;
    }
    return -1;
}

 int main(){
//    int x;
     generateKey_HMAC(128, "keyx");
//     hashing_HMAC("data", "key", "hmac1");
//     x = verify_HMAC("data", "key", "hmac1");
//     if(x == 1){
//         printf("verified!");
//     }

    unsigned char* m = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.";
////     unsigned char* m = "Sed ut perspiciatis unde omnis iste natus error sit voluptatem accusantium doloremque laudantium, totam rem aperiam, eaque ipsa quae ab illo inventore veritatis et quasi architecto beatae vitae dicta sunt explicabo. Nemo enim ipsam voluptatem quia voluptas sit aspernatur aut odit aut fugit, sed quia consequuntur magni dolores eos qui ratione voluptatem sequi nesciunt. Neque porro quisquam est, qui dolorem ipsum quia dolor sit amet, consectetur, adipisci velit, sed quia non numquam eius modi tempora incidunt ut labore et dolore magnam aliquam quaerat voluptatem. Ut enim ad minima veniam, quis nostrum exercitationem ullam corporis suscipit laboriosam, nisi ut aliquid ex ea commodi consequatur? Quis autem vel eum iure reprehenderit qui in ea voluptate velit esse quam nihil molestiae consequatur, vel illum qui dolorem eum fugiat quo voluptas nulla pariatur?";
     unsigned char *x, *y;
//
     x = hashmd5(m);
     y = hashmd5(x);
     printf("Digest: ");
     for (int i = 0; i < 16; ++i) {
         printf("%02x", y[i]);
     }
     printf("\n");


     return 0;
 }