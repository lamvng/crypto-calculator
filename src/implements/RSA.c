#include "../libs/RSA.h"
#include <stdio.h>
#include "gmp.h"
#include "time.h"

int generateKey_RSA(mpz_t e, mpz_t d, mpz_t n) {
  // SHOULD WE CHECK THE PRIMALITY OF E ??


  mpz_t p, q, temp, for_gcd, p_1, q_1, phi_n;
  mpz_inits(p,q, temp, for_gcd, p_1, q_1, phi_n , NULL);

  int length_RSA= 1024;
  int length2 = length_RSA / 2; //length for p and q

  // random creation
  gmp_randstate_t generator;
  gmp_randinit_default(generator);
  int t = time(NULL);
  gmp_randseed_ui(generator,t);

  mpz_set_ui(p, 4); //init value (temporary)
  mpz_set_ui(q, 4); //init value (temporary)

  do{
    // Part : Generate at random a k/2-bit prime p such that gcd(e,p-1) = 1 (512 bits)
    while((mpz_probab_prime_p(p,100) == 0) || (mpz_cmp_ui(for_gcd,1) !=0 )){
      mpz_urandomb(p, generator, length2-1);
      mpz_ui_pow_ui(temp,2, length2-1);
      mpz_add(p, p, temp);
      mpz_sub_ui(p_1, p, 1);
      mpz_gcd(for_gcd, p_1, e);
    }

    // Part : Generate at random a k/2-bit prime p such that gcd(e,p-1) = 1 (512 bits)
    while((mpz_probab_prime_p(q,15) == 0) || (mpz_cmp_ui(for_gcd,1) !=0 )){
      mpz_urandomb(q, generator, length2-1);
      mpz_ui_pow_ui(temp,2, length2-1);
      mpz_add(q, q, temp);
      mpz_sub_ui(q_1, q, 1);
      mpz_gcd(for_gcd, q_1, e);
    }

    // Part : n = pq
    mpz_mul(n,p,q);

    //Part : phi(n) = (p-1)(q-1)
    mpz_mul(phi_n, p_1, q_1);

  }while((mpz_sizeinbase(n,2)!=length_RSA) && (mpz_sizeinbase(phi_n,2) != length_RSA ) && (mpz_cmp(p,q) == 0)); //check that p and q are different

  //Part : d = e⁻1 mod phi(n)
  mpz_invert(d, e, phi_n);

  printf(" === RSA STANDARD KEY GENERATION ===\n" );
  gmp_printf("q = %Zd and size q = %d \n", q, mpz_sizeinbase(q,2));
  gmp_printf("p = %Zd and size p = %d \n", p, mpz_sizeinbase(p,2));
  gmp_printf("n = %Zd and size n = %d\n", n, mpz_sizeinbase(n,2));
  gmp_printf("phi_n = %Zd and size = %d\n", phi_n, mpz_sizeinbase(phi_n,2));
  gmp_printf("d = %Zd \n", d);


    //Publish pk={n,e}  /  Keep secret sk={d}
}


int generateKEY_RSA_CRT(mpz_t n, mpz_t e, mpz_t p, mpz_t q, mpz_t dp, mpz_t dq, mpz_t Ip){
  mpz_t temp, for_gcd, p_1, q_1;
  mpz_inits( temp, for_gcd, p_1, q_1, NULL);

  int length_RSA= 1024;
  int length2 = length_RSA / 2; //length for p and q

  // random creation
  gmp_randstate_t generator;
  gmp_randinit_default(generator);
  int t = time(NULL);
  gmp_randseed_ui(generator,t);

  mpz_set_ui(p, 4); //init value (temporary)
  mpz_set_ui(q, 4); //init value (temporary)

  do{
    // Part : Generate at random a k/2-bit prime p such that gcd(e,p-1) = 1 (512 bits)
    while((mpz_probab_prime_p(p,100) == 0) || (mpz_cmp_ui(for_gcd,1) !=0 )){
      mpz_urandomb(p, generator, length2-1);
      mpz_ui_pow_ui(temp,2, length2-1);
      mpz_add(p, p, temp);
      mpz_sub_ui(p_1, p, 1);
      mpz_gcd(for_gcd, p_1, e);
    }

    // Part : Generate at random a k/2-bit prime p such that gcd(e,p-1) = 1 (512 bits)
    while((mpz_probab_prime_p(q,15) == 0) || (mpz_cmp_ui(for_gcd,1) !=0 )){
      mpz_urandomb(q, generator, length2-1);
      mpz_ui_pow_ui(temp,2, length2-1);
      mpz_add(q, q, temp);
      mpz_sub_ui(q_1, q, 1);
      mpz_gcd(for_gcd, q_1, e);
    }

    // Part : n = pq
    mpz_mul(n,p,q);

  }while((mpz_sizeinbase(n,2)!=length_RSA) && (mpz_cmp(p,q) == 0)); //check that p and q are different

  //Part : dp = e⁻1 mod (p-1)
  mpz_invert(dp, e, p_1);

  //Part : dq = e⁻1 mod (q-1)
  mpz_invert(dq, e, q_1);

  //Part : Ip = p⁻1 mod q
  mpz_invert(Ip, p, q);


}


int encrypt_RSA(mpz_t m, mpz_t c, mpz_t e, mpz_t n){
  //mpz_t d;
  //mpz_init_set_ui(d,1);
  //generateKey_RSA(e,d,n); //key recovery

  printf(" === ENCRYPTION RSA === \n");

  mpz_powm(c, m, e, n); //m^e mod n
  gmp_printf("c = %Zd\n", c);

}

int encrypt_RSA_CRT(mpz_t m, mpz_t c, mpz_t e, mpz_t n){
  encrypt_RSA(c,m,e,n);
}


int decrypt_RSA(mpz_t m, mpz_t c, mpz_t d, mpz_t n) {
  mpz_t e;
  mpz_init_set_ui(e,5);
  generateKey_RSA(e,d,n); //key recovery

  printf(" === DECRYPTION RSA === \n");

  mpz_powm(m, c, d, n);
  gmp_printf("m = %Zd\n", m);


}

int decrypt_RSA_CRT(mpz_t m, mpz_t c, mpz_t dp, mpz_t dq, mpz_t Ip, mpz_t p, mpz_t q){
  //Slide 16 of RSA course

  mpz_t mp, mq, mq_mp, mq_mp_Ip, temp, temp2;
  mpz_inits(mp, mq, mq_mp, mq_mp_Ip, temp, temp2,NULL);

  //mp = c^dp mod p
  mpz_powm(mp, c, dp, p);

  //mq = c^dq mod q
  mpz_powm(mq, c, dq, q);

  //Garner's formula -> m = mp + p * ((mq-mp)*Ip mod q)
  mpz_sub(mq_mp, mq, mp); //(mq-mp)
  mpz_mul(mq_mp_Ip, mq_mp, Ip); //(mq-mp)*Ip
  mpz_mod(temp, mq_mp_Ip, q); // (mq-mp)*Ip mod q
  mpz_mul(temp2, p, temp);// p*((mq-mp)*Ip mod q)
  mpz_add(m, mp,temp2);  //m = mp + p * ((mq-mp)*Ip mod q)



  gmp_printf("mp = %Zd\n", mp);
  gmp_printf("mq = %Zd\n", mq);
  gmp_printf("mq_mp = %Zd\n", mq_mp);
  gmp_printf("mq_mp_Ip = %Zd\n", mq_mp_Ip);
  gmp_printf("(mq-mp)*Ip mod q = %Zd\n", temp);
  gmp_printf("p*((mq-mp)*Ip mod q) = %Zd\n", temp2);
  gmp_printf("m = %Zd\n", m);

}


int sign_RSA(mpz_t m, mpz_t c, mpz_t d, mpz_t n) {
    //Cours Gaborit
    // Pour signer un message m, Alice calcule m' = h(m) pour une fonction de hachage h sans collision. Elle détermine s = (m)^d (mod N) avec sa clef privée
}

int sign_RSA_CRT(mpz_t m, mpz_t c, mpz_t dp, mpz_t dq, mpz_t Ip, mpz_t p, mpz_t q){}



int verify_RSA(mpz_t m, mpz_t c, mpz_t e, mpz_t n) {
    // Cours Gaborit
    // Bob reçoit un couple (m',s'). Pour vérifier la signature, il teste si h(m')=(s')^e (mond N) avec la clef publique d'Alice
}

int verify_RSA_(mpz_t m, mpz_t c, mpz_t e, mpz_t n) {}

int main(){
  /* PART 1
  mpz_t e, n, d;
  mpz_inits(e,n,d,NULL);

  mpz_set_ui(e,17);
  mpz_set_ui(d,1);
  mpz_set_ui(n,1);

  generateKey_RSA(e,d,n);*/

  /* PART 2
  mpz_t m1, c1, e, n1;
  mpz_inits(m1, c1, e, n1, NULL);

  mpz_set_ui(m1,456);
  mpz_set_ui(c1, 0);
  mpz_set_ui(e,5);
  mpz_set_ui(n1,0);
  encrypt_RSA(m1,c1,e,n1);*/

  /*PART 3
  mpz_t m2, c2, d, n2;
  mpz_inits(m2, c2, d, n2, NULL);

  mpz_set_ui(m2,0);
  mpz_set_ui(c2, 562);
  mpz_set_ui(d,0);
  mpz_set_ui(n2,0);
  decrypt_RSA(m2, c2, d, n2);*/

  /* Decrypt RSA CRT Mode */
  mpz_t m, c, dp, dq, Ip, p, q;
  mpz_inits(m,c, dp, dq, Ip, p, q, NULL);

  mpz_set_ui(p, 547);
  mpz_set_ui(q, 797);
  mpz_set_ui(dp, 149);
  mpz_set_ui(dq, 579);
  mpz_set_ui(Ip, 424);
  mpz_set_ui(c, 77111);
  decrypt_RSA_CRT(m, c, dp,  dq, Ip, p, q);

  //encrypt();
  return 0;

}
