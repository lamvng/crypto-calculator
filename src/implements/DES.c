#include "../libs/DES.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TAILLE_CLE 8

typedef unsigned char tab;
typedef tab cle[TAILLE_CLE];

typedef struct {
    tab *data;
    int len;
} String;

int IP[] = {
    58, 50, 42, 34, 26, 18, 10, 2, 60,
    52, 44, 36, 28, 20, 12, 4, 62, 54,
    46, 38, 30, 22, 14, 6, 64, 56, 48,
    40, 32, 24, 16, 8, 57, 49, 41, 33,
    25, 17, 9, 1, 59, 51, 43, 35, 27,
    19, 11, 3, 61, 53, 45, 37, 29, 21,
    13, 5, 63, 55, 47, 39, 31, 23, 15, 7
};

int PC1[] = {
    57, 49, 41, 33, 25, 17, 9,
     1, 58, 50, 42, 34, 26, 18,
    10, 2, 59, 51, 43, 35, 27,
    19, 11, 3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
     7, 62, 54, 46, 38, 30, 22,
    14, 6, 61, 53, 45, 37, 29,
    21, 13, 5, 28, 20, 12, 4
};

int PC2[] = {
    14, 17, 11, 24, 1, 5,
     3, 28, 15, 6, 21, 10,
    23, 19, 12, 4, 26, 8,
    16, 7, 27, 20, 13, 2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32
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

int S[][64] = {
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

int IP2[] = {
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41, 9, 49, 17, 57, 25
};

int SHIFTS[] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};


/*
binaire vers hexa
*/
static char toHex(int b) {
    char o;

    //printf("\n--- %i ---\n",in);
    if (b >= 0x00 && b < 0x0A) {
        o = '0' + b;
      //  printf("\nsortie1 %i--- 0x%02hhx ---\n",in,o);
        return o;
    }
    if (b >= 0x0A && b <= 0x0F){
        o ='A' + b - 0x0A;
        //printf("\nsortie2 %i--- 0x%02hhx ---\n",in,in);
        return o;
    }
    return 0;

}

/*
retourne la valeur d'un tableau à un index donné
*/
static int getVal(const tab *mat, int index) {
    int i = index / 8;
    int bit = 7 - index % 8;
    return (mat[i] & (1 << bit)) != 0;
}


/*
tableau d'octet en chaine de caractère
en entrée : le tableau d'octet, le nombre d'octet
*/
static void toString(const tab *tabOct, int len, char *result) {
    while (len-- > 0) {
       // printf("====================%c",*tabOct);
        *result++ = toHex(*tabOct >> 4);
        *result++ = toHex(*tabOct & 0x0F);
        tabOct++;
    }
    *result = 0;
}


/*
mettre a jour une valeur dans un tableau
dst: le tableau, index : l'index que l'on veux modifier, value, la valeur que l'on souhaite entrer
*/
static void insert(tab *dst, int index, int value) {
    int ind2 = index / 8;
    int bit = 7 - index % 8;
    if (value == 0) {
        dst[ind2] = dst[ind2] & (~(1 << bit));
    } else {
        dst[ind2] = dst[ind2] | (1 << bit);
    }
}


/*
change un tableau d"octet en declant les bits vers la gauche en fonction du nombre de case entrée
src: le tableau
len: longueur du tableau
times: le nombre de positions a décaler
dst: le tableau de destination
*/
static void decal(const tab *src, int len, int pos, tab *dst) {

    for (int t = 1; t <= pos; ++t) {
        int temp = getVal(dst, 0);
        for (int i = 1; i <= len; ++i) {
            insert(dst, i - 1, getVal(dst, i));
        }
        insert(dst, len - 1, temp);
    }
    for (int i = 0; i <= len; i++) {
        insert(dst, i, getVal(src, i));
    }
}


/*
Calcule les sous clés pour DES
*/
typedef tab sClefs[17][6]; /* 17 sets of 48 bits */
static void calcCle(const cle key, sClefs ks) {
    tab c[17][7];
    tab d[17][4];
    tab kp[7];

    memset(c, 0, sizeof(c));
    memset(d, 0, sizeof(d));
    memset(ks, 0, sizeof(sClefs));

    for (int i = 0; i < 56; ++i) {
        insert(kp, i, getVal(key, PC1[i] - 1));
    }
    for (int i = 0; i < 28; ++i) {
        insert(c[0], i, getVal(kp, i));
        insert(d[0], i, getVal(kp, i + 28));
    }
    for (int i = 1; i < 17; ++i) {
        decal(c[i - 1], 28, SHIFTS[i - 1], c[i]);
        decal(d[i - 1], 28, SHIFTS[i - 1], d[i]);
    }
    for (int i = 1; i < 17; ++i) {
        for (int j = 28; j < 56; ++j) {
            insert(c[i], j, getVal(d[i], j - 28));
        }
    }
    for (int i = 1; i < 17; ++i) {
        for (int j = 0; j < 48; ++j) {
            insert(ks[i], j, getVal(c[i], PC2[j] - 1));
        }
    }
}



/*
r : tableau d'octet a traiter
ks une des sous clés a utiliser pour le chiffrement
sp : resultat
*/
static void f(tab *r, tab *ks, tab *sp) {
    tab er[6]; /* 48 bits */
    tab sr[4]; /* 32 bits */
    int i;

    memset(er, 0, sizeof(er));
    memset(sr, 0, sizeof(sr));

    for (i = 0; i < 48; ++i) {
        insert(er, i, getVal(r, E[i] - 1));
    }

    for (i = 0; i < 6; ++i) {
        er[i] ^= ks[i];
    }

    for (i = 0; i < 8; ++i) {
        int j = i * 6;
        int b[6];
        int k, row, col, m, n;

        for (k = 0; k < 6; ++k) {
            b[k] = getVal(er, j + k) != 0 ? 1 : 0;
        }

        row = 2 * b[0] + b[5];
        col = 8 * b[1] + 4 * b[2] + 2 * b[3] + b[4];
        m = S[i][row * 16 + col]; /* apply table s */
        n = 1;

        while (m > 0) {
            int p = m % 2;
            insert(sr, (i + 1) * 4 - n, p == 1);
            m /= 2;
            n++;
        }
    }

    for (i = 0; i < 32; ++i) {
        insert(sp, i, getVal(sr, P[i] - 1));
    }
}

/*

*/
static void algDes(const tab *message, sClefs ks, tab *ep) {
    tab prtG[17][4];
    tab prtD[17][4];
    tab mp[8];
    tab e[8];

    for (int i = 0; i < 64; ++i) {
        insert(mp, i, getVal(message, IP[i] - 1));
    }

    for (int i = 0; i < 32; ++i) {
        insert(prtG[0], i, getVal(mp, i));
        insert(prtD[0], i, getVal(mp, i + 32));
    }
    for (int i = 1; i < 17; ++i) {
        tab ke[4];
        memcpy(prtG[i], prtD[i - 1], 4);
        f(prtD[i - 1], ks[i], ke);
        for (int j = 0; j < 4; ++j) {
            prtG[i - 1][j] ^= ke[j];
        }
        memcpy(prtD[i], prtG[i - 1], 4);
    }

    for (int i = 0; i < 32; ++i) {
        insert(e, i, getVal(prtD[16], i));
    }
    for (int i = 32; i < 64; ++i) {
        insert(e, i, getVal(prtG[16], i - 32));
    }

    for (int i = 0; i < 64; ++i) {
        insert(ep, i, getVal(e, IP2[i] - 1));
    }
}

String encrypt(const cle key, const tab *message, int len) {
    String result = { 0, 0 };
    sClefs ks;
    tab byte;

    calcCle(key, ks);
    byte = 8 - len % 8;
    result.len = len + byte;
    result.data = (tab*)malloc(result.len);
    memcpy(result.data, message, len);
    memset(&result.data[len], byte, byte);

    for (int i = 0; i < result.len; i += 8) {
        algDes(&result.data[i], ks, &result.data[i]);
    }
    return result;
}


String decrypt(const cle key, const tab *message, int len) {
    String result = { 0, 0 };
    sClefs ks;
    tab padByte;

    calcCle(key, ks);

    for (int i = 1; i < 9; ++i) {
        for (int j = 0; j < 6; ++j) {
            tab temp = ks[i][j];
            ks[i][j] = ks[17 - i][j];
            ks[17 - i][j] = temp;
        }
    }

    result.data = (tab*)malloc(len);
    memcpy(result.data, message, len);
    result.len = len;
    for (int i = 0; i < result.len; i += 8) {
        algDes(&result.data[i], ks, &result.data[i]);
    }

    padByte = result.data[len - 1];
    result.len -= padByte;
    return result;
}


/*
 * afficher resultat et etapes de DES
 */
void DES(const cle key, const tab *message, int len) {
    String chiff, clair;
    char buffer[128];

    toString(key, TAILLE_CLE, buffer);
    printf("cle     : %s\n", buffer);
    printf("message initial : ");
    for(int i=0; i< len; i++){
      printf("%c",message[i]);
    }
    printf("\n");

    toString(message, len, buffer);
    printf("Message en hexa: %s\n", buffer);
    chiff = encrypt(key, message, len);
    toString(chiff.data, chiff.len, buffer);
    printf("chiffre : %s\n", buffer);
    clair = decrypt(key, chiff.data, chiff.len);
    toString(clair.data, clair.len, buffer);

    printf("decode en hexa: %s\n", buffer);
    printf("message final : ");
    for(int i=0; i< clair.len; i++){
      printf("%c",clair.data[i]);
    }
    printf("\n\n");

    free(chiff.data);
    chiff.data = 0;
    free(clair.data);
    clair.data = 0;
}

int main() {
    const cle clefs[] = {
        {0x4c, 0x61, 0x63, 0x6c, 0xe9, 0x44, 0x45, 0x53},
        {0x44, 0x45, 0x53, 0x63, 0x6c, 0xe9, 0x4c, 0x41}
    };
    const tab message1[] = { 'h', 'e', 'l','l','o','c','r','y','p','t','i','s' };
    const tab message2[] = { 0x63 ,0x61 ,0x6c ,0x63 ,0x75 ,0x6c ,0x61 ,0x74 ,0x72 ,0x69 ,0x63 ,0x65 ,0x20 ,0x63 ,0x72 ,0x79 ,0x70 ,0x74 ,0x6f ,0x67 ,0x72 ,0x61 ,0x70 ,0x68 ,0x69 ,0x71 ,0x75 ,0x65 };
    int len;

    len = sizeof(message1) / sizeof(tab);
    DES(clefs[0], message1, len);

    len = sizeof(message2) / sizeof(tab);
    DES(clefs[1], message2, len);
    return 0;
}
