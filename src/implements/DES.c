#include "../libs/DES.h"
#include "../libs/libBinChar.h"
//#include "libBinChar.c"
#include <stdbool.h>
#include <math.h>
#include <time.h>

char resultatFinalafP[64];
char resultatFinalafPD[64];
char solution[64] = {0};
char final[16][48];


int IP[64] = {
  58, 50, 42, 34, 26, 18, 10, 2, 60,
  52, 44, 36, 28, 20, 12, 4, 62, 54,
  46, 38, 30, 22, 14, 6, 64, 56, 48,
  40, 32, 24, 16, 8, 57, 49, 41, 33,
  25, 17, 9, 1, 59, 51, 43, 35, 27,
  19, 11, 3, 61, 53, 45, 37, 29, 21,
  13, 5, 63, 55, 47, 39, 31, 23, 15, 7
};

int E[] = {
    32, 1, 2, 3, 4, 5,
     4, 5, 6, 7, 8, 9,
     8, 9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32, 1
};

int cp1[] = {
    57, 49, 41, 33, 25, 17, 9,
     1, 58, 50, 42, 34, 26, 18,
    10, 2, 59, 51, 43, 35, 27,
    19, 11, 3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
     7, 62, 54, 46, 38, 30, 22,
    14, 6, 61, 53, 45, 37, 29,
    21, 13, 5, 28, 20, 12, 4
};

int cp2[] = {
    14, 17, 11, 24, 1, 5,
     3, 28, 15, 6, 21, 10,
    23, 19, 12, 4, 26, 8,
    16, 7, 27, 20, 13, 2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32
};

int LS[] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};



int sbox[][64] = {
    {
        14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
        0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
         4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
        15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13
    },
    {
        15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
         3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
         0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
        13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9
    },
    {
        10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
        13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
        13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
         1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12
    },
    {
         7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
        13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
        10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
         3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14
    },
    {
         2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
        14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
         4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
        11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3
    },
    {
        12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
        10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
         9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
         4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13
    },
    {
         4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
        13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
         1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
         6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12
    },
    {
        13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
         1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
         7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
         2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11
    }
};


int P[] = {
    16, 7, 20, 21,
    29, 12, 28, 17,
     1, 15, 23, 26,
     5, 18, 31, 10,
     2, 8, 24, 14,
    32, 27, 3, 9,
    19, 13, 30, 6,
    22, 11, 4, 25
};

int PI1[] = {
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41, 9, 49, 17, 57, 25
};


/*
*
*/
void printMatr(char * ret, int size){
  int n = 0;
  for (int i = 0 ; i < size ; i++){
    printf(" %c ", ret[i]);
    n++;
    if(n == 6){
      printf("\n");
      n=0;
    }
  }
}


/*
*
*
*/
void printMatrint(char * ret, int size){
  int n = 0;
  for (int i = 0 ; i < size ; i++){
    printf(" %i ", ret[i]);
    n++;
    if(n == 6){
      printf("\n");
      n=0;
    }
  }
}

/*
*
*
*/
void decalGauche(char * init, int size, char * after){
  for(int i = 0; i < size; i++){
    after[i]=init[i+1];
  }
  after[size-1] = init[0];
}


/*
*
*
*/
void permut(char in1[], char out[], int perm[], int size){
  for(int i = 0; i < size ; i++){
    out[i] = in1[perm[i]-1];
  }
}


/*
*
*
*/
void genCle(char cle[]){
  char cp11[56];
  char G[28];
  char D[28];
  char GD[56];

  //permutation cp-1
  permut(cle, cp11, cp1, 56);

  for(int i = 0; i < 56; i++){

    if(i > 27){
      D[i-28]=cp11[i];
    }else{
      G[i]=cp11[i];
    }
  }

  for(int m = 0; m< 16; m++){
    int nbDecal = 0;
    char Gdecal[28];
    char Ddecal[28];
    nbDecal = LS[m];

    for(int i=0;i<nbDecal;i++){
      decalGauche(G, 28,Gdecal);
      decalGauche(D, 28,Ddecal);

      for( int i  = 0; i< 28; i++){
        G[i]=Gdecal[i];
        D[i]=Ddecal[i];
      }
    }

    for(int i=0; i < 56; i++){
      if(i>27){
        GD[i]=Ddecal[i-28];
      }else{
        GD[i]=Gdecal[i];
      }
    }

    char k[48];
    permut(GD,k,cp2,48);
    for(int i =0; i < 48; i++){
      final[m][i]=k[i];
    }
  }
}
/*
*
*/
void xor(char d[], char k[16][48], int it, char out[]){
  for(int i=0; i<48; i++){
      out[i]= d[i]^k[it][i];
  }
}

void xor32(char d[], char k[16][32], int it, char out[]){
  for(int i=0; i<32; i++){
      out[i]= d[i]^k[it][i];
  }
}

/*
*
*/
void scind(char d0ex[], char sc[8][6]){
  int comp=0;
  for(int y=0;y<8;y++){
    for(int n = 0; n< 6;n++){
      sc[y][n]=d0ex[comp];
      comp++;
    }
  }
}

/*
*
*/
int getLigne(int un, int deux){
  int ret = 0;
  if(deux == 1){
    ret = 2;
  }
  ret = ret + un;
  return ret;
}

/*
*
*
*/
int getCol(int un, int deux, int quatre, int huit){
  int ret = 0;

  if(deux == 1){
    ret = ret + 2;
  }
  if(quatre == 1){
    ret = ret + 4;
  }
  if(huit == 1){
    ret = ret + 8;
  }
  ret = ret + un;
  return ret;
}

/*
*
*/
void fsbox(char sc[8][6], char resultat[32]){

  int comptRes = 0;
  for(int nb = 0; nb< 8; nb++){
    int ligne = getLigne(sc[nb][5], sc[nb][0]);
    int col = getCol(sc[nb][4], sc[nb][3], sc[nb][2], sc[nb][1]);

    int result = sbox[nb][ligne*16+col];

    if(result>=8){
      //printf("1");
      resultat[comptRes] = 1;
      comptRes++;
      result = result -8;
    }else{
      resultat[comptRes] = 0;
      comptRes++;
      //printf("0");
    }
    if(result>=4){
      //printf("1");
      resultat[comptRes] = 1;
      comptRes++;
      result = result - 4;
    }else{
      resultat[comptRes] = 0;
      comptRes++;
      //printf("0");
    }
    if(result>=2){
      //printf("1");
      resultat[comptRes] = 1;
      comptRes++;
      result = result - 2;
    }else{
      resultat[comptRes] = 0;
      comptRes++;
      //printf("0");
    }
    if(result==1){
      //printf("1");
      resultat[comptRes] = 1;
      comptRes++;
      result = result - 1;
    }else{
      resultat[comptRes] = 0;
      comptRes++;
      //printf("0");
    }
  }
}


/*
*
*/
char * encrypt(char message[], char cle[]){
  char IP1[64];
  char d0[17][32];
  char g0[17][32];
  char resultatFinal[64];
  genCle(cle);

  //permutation initiale
  permut(message,IP1,IP,64);

  //scinde IP1 en g0 et d0 de 32bits
  for(int i=0; i < 64; i++){
    if(i > 31){
        d0[0][i-32]=IP1[i];
    }else{
        g0[0][i]=IP1[i];
    }
  }

  for(int it = 0; it < 16; it++){    //expansion D0 avec E

//    printf("\n ------etape numéro------ %i\n", it+1);

    for(int gd=0; gd< 32; gd++){
      g0[it+1][gd]=d0[it][gd];
    }

    char d0E[48];
    permut(d0[it],d0E,E,48);
    char d0ex[48];
    xor(d0E, final, it, d0ex);

    // on scinde en 8 bloc de 6 bits
    char sc[8][6];
    scind(d0ex, sc);
    char resultat[32];
    fsbox(sc,resultat);
    char resultatP[32];

    permut(resultat, resultatP,P ,32);
    //xor avec Gn-1
    xor32(resultatP,g0, it, d0[it+1]);
  }

  for(int g = 0; g< 32;g++){
    resultatFinal[g]=d0[16][g];
  }
  for(int d = 32; d< 64;d++){
    resultatFinal[d]=g0[15][d];
  }


  permut(resultatFinal,resultatFinalafP, PI1,64);

  int n = 0;
  for (int i = 0; i < 64; i++) {
      n += sprintf (&solution[n], "%c", resultatFinalafP[i]);
  }
  printf ("\n chiffré   s = %s\n", solution);

  printf("%s\n", BinaryToASCII(solution) );

  return solution;
}

char * decrypt(char message[],char cle[]){
  char IP1[64];
  char d0[17][32];
  char g0[17][32];
  char resultatFinal[64];

  genCle(cle);
  //permutation initiale
  permut(message,IP1,IP,64);


  //scinde IP1 en g0 et d0 de 32bits
  for(int i=0; i < 64; i++){
    if(i > 31){
        d0[0][i-32]=IP1[i];
    }else{
        g0[0][i]=IP1[i];
    }
  }

  for(int it = 0; it < 16; it++){    //expansion D0 avec E

    char d0E[48];
    permut(d0[it],d0E,E,48);

    char d0ex[48];
    xor(d0E, final, 15-it, d0ex);

    // on scinde en 8 bloc de 6 bits
    char sc[8][6];
    scind(d0ex, sc);

    char resultat[32];
    fsbox(sc,resultat);
    char resultatP[32];
    permut(resultat, resultatP ,P ,32);

    //xor avec Gn-1
    xor32(resultatP,g0, it, d0[it+1]);
    for(int gd=0; gd< 32; gd++){
      g0[it+1][gd]=d0[it][gd];
    }

  }

  for(int g = 0; g< 32;g++){
    resultatFinal[g]=d0[16][g];
  }
  for(int d = 32; d< 64;d++){
    resultatFinal[d]=g0[15][d];
  }

  permut(resultatFinal,resultatFinalafP, PI1,64);

  int n = 0;
  for (int i = 0; i < 64; i++) {
      n += sprintf (&solution[n], "%c", resultatFinalafP[i]);
  }
  printf ("\n déchiffré   s = %s\n", solution);

  printf("%s\n", BinaryToASCII(solution) );

  return BinaryToASCII(solution);
}

char * password;
/*
*
*/
char *generateKey_DES(){
  char* ret = malloc(64);
  srand(time(NULL));
  for (int i = 0; i < 64; i++) {
    int c = rand()%2;
    ret[i]=c;
  }
  //printMatrint(ret,64);
  return ret;
}
/*
int main(){
    char * cle = generateKey_DES();
    char message1[8] = "abcdefgh";
    char* test = stringToBinary(message1);
    char cle64[8] = "password";
    char * cleBin = stringToBinary(cle64);
    char cle65[8] = "aaaaaaaa";
    char * cleBin2 = stringToBinary(cle65);

    decrypt(encrypt(test,cle),cle);


    return 0;
}
*/
