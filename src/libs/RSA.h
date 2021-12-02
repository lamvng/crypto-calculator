#include "gmp.h"

int generateKey_RSA(mpz_t e, mpz_t d, mpz_t n);
int encrypt_RSA(mpz_t m, mpz_t c, mpz_t e, mpz_t n);
int decrypt_RSA(mpz_t m, mpz_t c, mpz_t d, mpz_t n);
int sign_RSA(mpz_t m, mpz_t c, mpz_t d, mpz_t n);
int verify_RSA(mpz_t m, mpz_t c, mpz_t e, mpz_t n);
