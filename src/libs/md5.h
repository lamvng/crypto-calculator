#include "gmp.h"

<<<<<<< HEAD
// // Bit manipulation on array
// // http://www.mathcs.emory.edu/~cheung/Courses/255/Syllabus/1-C-intro/bit-array.html
// // This time, each element (unsigned char) is 1 byte = 8 bits
// #define setBit(A,k)     ( A[(k/8)] |= (1 << (k%8)) )
// #define clearBit(A,k)   ( A[(k/8)] &= ~(1 << (k%8)) )
// #define testBit(A,k)    ( A[(k/8)] & (1 << (k%8)) )


unsigned char* readBinary(char* filename);
unsigned char* hashmd5(unsigned char* data_buffer, unsigned int total_size);
=======
// Input: A binary STRING
// Output: A binary STRING of 128 bits (hash)

char* hashmd5();
>>>>>>> main
