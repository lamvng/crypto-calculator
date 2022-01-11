

// // Bit manipulation on array
// // http://www.mathcs.emory.edu/~cheung/Courses/255/Syllabus/1-C-intro/bit-array.html
// // This time, each element (unsigned char) is 1 byte = 8 bits
// #define setBit(A,k)     ( A[(k/8)] |= (1 << (k%8)) )
// #define clearBit(A,k)   ( A[(k/8)] &= ~(1 << (k%8)) )
// #define testBit(A,k)    ( A[(k/8)] & (1 << (k%8)) )

// Read input file
// Output: (Array unsigned char*) containing contents of file
unsigned char* readBinary(char* filename);



// Input: Array (char*) of input
// Example input: char file_buffer[] = "They are deterministic";
// Output hash: array of (unsigned char*) with 16 elements
// Output hash size: 16 * 8 = 128 bits
unsigned char* hashmd5(unsigned char* file_buffer);
