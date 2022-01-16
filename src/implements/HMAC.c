#include "../libs/HMAC.h"
#include "../libs/md5.h"
#include "gmp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <openssl/md5.h>
#define char_to_hex(c) ( (toupper((c)) >= 'A' && toupper((c)) <= 'F')? toupper((c)) - 'A' + 10 : (c) - '0')

#define HASH_SIZE 16

int generateKey_HMAC(int keySize, char* keyFileName){
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
    return 1;
}

unsigned char* read_file(FILE *fp, long file_size){
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
//    tkey = (unsigned char*) malloc(16 * sizeof(unsigned char));

    //Hash key if len key > 64
    if(key_size > 64){
        //test
        printf("key size exceeded\n");
//        MD5_CTX kctx;
//        MD5_Init(&kctx);
//        MD5_Update(&kctx, hmac_key, key_size);
//        MD5_Final(tkey, &kctx);
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

    unsigned char *digest;
//    digest = (unsigned char*)malloc(sizeof(char)*HASH_SIZE);
    //Inner MD5: MD5(K_ipad || text)

//    MD5_CTX ctx;
//    MD5_Init(&ctx);
//    MD5_Update(&ctx, k_ipad, 64);
//    MD5_Update(&ctx, file_content, file_size);
//    MD5_Final(digest, &ctx);

    //Concat k_ipad & file_content
    unsigned char inner_content[file_size+64];
    memcpy(inner_content, k_ipad, 64);
    memcpy(inner_content+64, file_content, file_size);
    digest = hashmd5(inner_content);



    //Outer MD5: MD5(k_opad||innerMD5)
//    MD5_Init(&ctx);
//    MD5_Update(&ctx, k_opad, 64);
//    MD5_Update(&ctx, digest, 16);
//    MD5_Final(digest, &ctx);
    unsigned char outer_content[HASH_SIZE+64];
    memcpy(outer_content, k_opad, 64);
    memcpy(outer_content+64, digest, HASH_SIZE);
    digest = hashmd5(outer_content);


    //Write hmac
    fp = fopen(hmacFileName, "w+");
    if(fp == NULL){
        perror("Failed: ");
        return -1;
    }

    fputs((char*)digest, fp);
    fclose(fp);

    return 0;
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

    strcpy(hmac_file, hmacFileName);
    strcat(hmac_file, "_temp");
    if(hashing_HMAC(fileName, keyFileName, hmac_file) != 0){
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
        printf("HMAC verified!\n");
    }
    return 0;
}

 int main(){
     generateKey_HMAC(128, "keyx");
     hashing_HMAC("data", "keyx", "hmac1");
     verify_HMAC("data", "keyx", "hmac1");
     return 0;
 }