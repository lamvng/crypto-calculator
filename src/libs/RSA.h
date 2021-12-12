#include "gmp.h"

int generateKey_RSA(mpz_t e, mpz_t d, mpz_t n);
int generateKey_RSA_CRT(mpz_t n, mpz_t e, mpz_t p, mpz_t q, mpz_t dp, mpz_t dq, mpz_t Ip);

int encrypt_RSA(mpz_t m, mpz_t c, mpz_t e, mpz_t n);
int encrypt_RSA_CRT(mpz_t m, mpz_t c, mpz_t e, mpz_t n);

int decrypt_RSA(mpz_t m, mpz_t c, mpz_t d, mpz_t n);
int decrypt_RSA_CRT(mpz_t m, mpz_t c, mpz_t dp, mpz_t dq, mpz_t Ip, mpz_t p, mpz_t q);

int sign_RSA(mpz_t m, mpz_t c, mpz_t d, mpz_t n);
int sign_RSA_CRT(mpz_t m, mpz_t c, mpz_t dp, mpz_t dq, mpz_t Ip, mpz_t p, mpz_t q);

int verify_RSA(mpz_t m, mpz_t c, mpz_t e, mpz_t n);
int verify_RSA_CRT(mpz_t m, mpz_t c, mpz_t e, mpz_t n);
