#include "../libs/RSA.h"
#include <stdio.h>
#include "gmp.h"

int generateKey_RSA(mpz_t e, mpz_t d) {
    // TO DO
}

int encrypt_RSA(mpz_t m, mpz_t c, mpz_t e, mpz_t n){
    // TO DO
}

int decrypt_RSA(mpz_t m, mpz_t c, mpz_t d, mpz_t n) {
    // TO DO
}


int sign_RSA(mpz_t m, mpz_t c, mpz_t d, mpz_t n) {
    //TO DO
}


int verify_RSA(mpz_t m, mpz_t c, mpz_t e, mpz_t n) {
    // TO DO
}