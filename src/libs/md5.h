

// // Bit manipulation on array
// // http://www.mathcs.emory.edu/~cheung/Courses/255/Syllabus/1-C-intro/bit-array.html
// // This time, each element (unsigned char) is 1 byte = 8 bits
// #define setBit(A,k)     ( A[(k/8)] |= (1 << (k%8)) )
// #define clearBit(A,k)   ( A[(k/8)] &= ~(1 << (k%8)) )
// #define testBit(A,k)    ( A[(k/8)] & (1 << (k%8)) )

// Read input file
// Output: Array unsigned char*
unsigned char* readBinary(char* filename);



// Input: Array unsigned char* of input
// Output: unsigned int* of 128 bits - output hash
unsigned int* hashmd5(unsigned char* data_buffer);
