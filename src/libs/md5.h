// Read input file
// Output: (Array unsigned char*) containing contents of file
unsigned char* readBinary(char* filename);



// Input: Array (unsigned char*) of input
// Example input: unsigned char file_buffer[] = "Lorem ipsum dolor sit amet"
// Output hash: array of (unsigned char*) with 16 elements
// Output hash size: 16 * 8 = 128 bits
unsigned char* hashmd5(unsigned char* file_buffer);