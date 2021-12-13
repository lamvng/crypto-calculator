#include "../libs/md5.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <math.h>


#define BLOCK_SIZE_BYTE 64 // Block size for hashing = 512 bits = 64 bytes
#define LAST_BLOCK_SIZE_BYTE 56 // If data of last block size = 56 bytes --> An entire new block must be add just for padding
#define BLOCK_SIZE_INT (BLOCK_SIZE_BYTE / 4)
#define NUM_BITS_IN_INT (sizeof(int) * 8)

// Assign value to a bit position k in an array
// http://www.mathcs.emory.edu/%7Echeung/Courses/255/Syllabus/1-C-intro/bit-array.html
void toggleBit(unsigned char* array, unsigned int value, long unsigned int k) {
    unsigned int i = k / 8; // char array index array[i]
    unsigned int pos = k % 8; // bit position in array[i] // Count from LSB, not MSB
    unsigned int flag = 1;

    // Set bit
    if (value == 1) {
        flag = flag << pos; // Left shift to set bit position
        array[i] = array[i] | flag;
    }
    
    // Clear bit
    else if (value == 0) {
        flag = flag << pos; // Left shift to set bit position
        flag = ~flag; // Inverse the flag
        array[i] = array[i] & flag;
    }

}
unsigned int findSize(const char* file_name)
{
    struct stat st; /*declare stat variable*/

    /*get the size using stat()*/
    if (stat(file_name, &st) == 0)
        return (st.st_size);
    else 
        return -1;
}


// Return the n-th byte of an integer (byte 0 counts as the LSB)
// long int = 64 bits --> Shift 7 bytes maximum, shift 8 bytes will return the first byte
unsigned int getByte(long unsigned int number, unsigned no_byte) {
    return (number >> (no_byte * 8)) & 0xFF;    // Mask and return the required byte
}


// Read data from file
unsigned char* readBinary(char* input_file) {

    unsigned int file_size;

    // Determine file size
    file_size = findSize(input_file);
    if (file_size == -1) {
        printf("Problem while reading file.\n");
        exit(EXIT_FAILURE);
    }

    // Allocate buffer
    unsigned char* file_buffer = (unsigned char*) malloc(file_size * sizeof(unsigned char));
    // BUG: Buffer do not allocate file_size elements

    // Read file
    FILE *ptr;
    ptr = fopen(input_file, "rb");
    fread(file_buffer, sizeof(unsigned char), file_size, ptr);
    fclose(ptr);

    return file_buffer;
}


// Calculate total data size after padding (in bytes)
unsigned int getDataSize(unsigned char* file_buffer, unsigned int file_size) {
    unsigned int file_size_padding, num_block, size_last_block, total_size;
    long unsigned int file_size_padding_bit; // 64 bits

    // Determine the number of necessary blocks
    num_block = (unsigned int) ceil ((double)file_size / BLOCK_SIZE_BYTE);
    size_last_block = file_size % BLOCK_SIZE_BYTE;

    // If data of last block size = 56 bytes --> An entire new block must be add just for padding
    // Or if size_last_block == 0 --> Add new block anyways
    if (size_last_block >= LAST_BLOCK_SIZE_BYTE || size_last_block == 0) {
        num_block += 1;
    }

    total_size = num_block * BLOCK_SIZE_BYTE;

    return total_size;
}


// Padding data
unsigned char* padding(unsigned char* file_buffer, unsigned int file_size, unsigned int total_size) {

    unsigned int i;
    unsigned int file_size_padding;
    long unsigned int file_size_padding_bit;

    // Pad data_buffer

    // Allocate array to store data of all blocks
    unsigned char* data_buffer = (unsigned char*) malloc(total_size * sizeof(unsigned char)); // Return a copy of file_buffer with more space
    if (!data_buffer) {
        printf("Cannot allocate buffer.\n");
        exit(EXIT_FAILURE);
    }

    // Pad the first bits as data
    for (i=0; i<file_size; i++) {
        data_buffer[i] = file_buffer[i];
    }

    // The next bit is 1
    // It is the MSB of the next byte
    long unsigned int pad_bit = file_size*8 + 7;
    toggleBit(data_buffer, 1, pad_bit);

    // Get file size in 64 bits = 8 bytes
    // AND with 0xFF (2 F - since file_size_padding is in bytes)
    file_size_padding = (file_size & 0xFF);

    // Pad the last 64 bits (8 bytes) as data size
    file_size_padding_bit = file_size_padding << 3; // Data size in bit

    unsigned int byte_position = 0;
    for (i=total_size-1; i>=total_size-8; i--) {
        data_buffer[i] = getByte(file_size_padding_bit, byte_position);
        byte_position ++;
    }

    return data_buffer;
}


// Convert char* data_buffer array to int* buffer array after padding
// For later calculation on int
unsigned int* convertUint (unsigned char* data_buffer, unsigned int total_size, unsigned int message_len) {
    unsigned int* message_all_block = (unsigned int*) malloc(message_len * sizeof(unsigned int));
    int i,j;
    for (i=0; i<total_size; i+=4){
        // Left shift to append hex value
        message_all_block[j] = ((unsigned int)data_buffer[i] << 24) | ((unsigned int)data_buffer[i+1] << 16) | ((unsigned int)data_buffer[i+2] << 8) | ((unsigned int)data_buffer[i+3]);
        j++;
    }
    return message_all_block;
}


// 4 functions F G H I to manipulate the buffer

// F (X, Y, Z) = (X & Y) | (~X & Z)
unsigned int F (unsigned int X, unsigned int Y, unsigned int Z) {
    return ((X & Y) | (~X & Z));
}

// G (X, Y, Z) = (X & Z) | (Y & ~Z)
unsigned int G (unsigned int X, unsigned int Y, unsigned int Z) {
    return ((X & Z) | (Y & ~Z));
}

// H (X, Y, Z) = X XOR Y XOR Z
unsigned int H (unsigned int X, unsigned int Y, unsigned int Z) {
    return (X ^ Y ^ Z);
}

// I (X, Y, Z) = Y XOR (X | ~Z)
unsigned int I (unsigned int X, unsigned int Y, unsigned int Z) {
    return (Y ^ (X | ~Z));
}


// Circular left shift in UNSIGNED INT variable
// https://stackoverflow.com/questions/7162454/circular-shifting-in-c
int rotleft(int num, int shift) {
    return (num << shift) | (num >> (NUM_BITS_IN_INT - shift));
}



// 4 Rounds function
// Divide the current block into 16 words --> No need to, message already has 16 elements of uint32
// return a
// a b c d: Buffer
// X : Message (32 bits) in block (512 bits)
// T[i] : Constant T, index i (T[i] = T[1..64])
unsigned int R1(unsigned int a, unsigned int b, unsigned int c, unsigned int d, unsigned int X, unsigned int* T, unsigned int s, unsigned int i) {
    return a + (rotleft(a + F(b, c, d) + X + T[i-1], s));
}

unsigned int R2(unsigned int a, unsigned int b, unsigned int c, unsigned int d, unsigned int X, unsigned int* T, unsigned int s, unsigned int i) {
    return a + (rotleft(a + G(b, c, d) + X + T[i-1], s));
}

unsigned int R3(unsigned int a, unsigned int b, unsigned int c, unsigned int d, unsigned int X, unsigned int* T, unsigned int s, unsigned int i) {
    return a + (rotleft(a + H(b, c, d) + X + T[i-1], s));
}

unsigned int R4(unsigned int a, unsigned int b, unsigned int c, unsigned int d, unsigned int X, unsigned int* T, unsigned int s, unsigned int i) {
    return a + (rotleft(a + I(b, c, d) + X + T[i-1], s));
}


// Load constant T (pre-calculated) from file
// T[1, 2, ..., 64]
// T[i] = int(abs(sin(i)) * 2**32)
unsigned int* getT(char* constant_t_file) {
    // Read file
    FILE *ptr;
    char* line = NULL;
    size_t len_line = 0;
    ssize_t read_line;
    unsigned int *T;
    int i = 0;
    T = (unsigned int*) malloc(64 * sizeof(unsigned int));

    ptr = fopen(constant_t_file, "r");
    if(ptr == NULL) {
        printf("Unable to open file!\n");
        exit(EXIT_FAILURE);
    }
    while ((read_line = getline(&line, &len_line, ptr)) != -1) {
        sscanf(line, "%x", &T[i]);
        i++;
    }
    if (i != 64) {
        printf("Problem loading constant T.\n");
        exit(EXIT_FAILURE);
    }
    fclose(ptr);
    return T;
}
      

// Get the current message block
unsigned int* getCurrentBlock(unsigned int* message_all_block, unsigned int i) {
    unsigned int* current_message = (unsigned int*) malloc(BLOCK_SIZE_INT * sizeof(unsigned int));
    for (int j=0; j<BLOCK_SIZE_INT; j++) {
        current_message[j] = message_all_block[i];
    }
    return current_message;
}

// TODO 2
// http://herongyang.com/Cryptography/MD5-Message-Digest-Algorithm-Overview.html
// Process each block of 512 bits = 16 elements of message_all_block

// A B C D: Buffer
// X : Message (32 bits) in block (512 bits)
// T[i] : Constant T, index i
// NOTE // Attention: In the algo, T[i] is T[1..64] --> Need conversion to T[0..63]
void processBlock (unsigned int* current_message, unsigned int* T, unsigned int A, unsigned int B, unsigned int C, unsigned int D) {
    unsigned int i;
    unsigned int AA, BB, CC, DD;

    // Round 1: 16 operations
    A = R1(A,B,C,D,current_message[0], T, 7, 1);
    D = R1(D,A,B,C,current_message[1],T, 12, 2);
    C = R1(C,D,A,B,current_message[2],T, 17, 3);
    B = R1(B,C,D,A,current_message[3],T, 22, 4);
    A = R1(A,B,C,D,current_message[4], T, 7, 5);
    D = R1(D,A,B,C,current_message[5],T, 12, 6);
    C = R1(C,D,A,B,current_message[6],T, 17, 7);
    B = R1(B,C,D,A,current_message[7],T, 22, 8);
    A = R1(A,B,C,D,current_message[8], T, 7, 9);
    D = R1(D,A,B,C,current_message[9], T, 12, 10);
    C = R1(C,D,A,B,current_message[10], T, 17, 11);
    B = R1(B,C,D,A,current_message[11], T, 22, 12);
    A = R1(A,B,C,D,current_message[12], T, 7, 13);
    D = R1(D,A,B,C,current_message[13], T, 12, 14);
    C = R1(C,D,A,B,current_message[14], T, 17, 15);
    B = R1(B,C,D,A,current_message[15], T, 22, 16);

     // Round 2: 16 operations
    A = R2(A,B,C,D,current_message[1], T, 5, 17);
    D = R2(D,A,B,C,current_message[6], T, 9, 18);
    C = R2(C,D,A,B,current_message[11], T, 14, 19);
    B = R2(B,C,D,A,current_message[0], T, 20, 20);
    A = R2(A,B,C,D,current_message[5], T, 5, 21);
    D = R2(D,A,B,C,current_message[10], T, 9, 22);
    C = R2(C,D,A,B,current_message[15], T, 14, 23);
    B = R2(B,C,D,A,current_message[4], T, 20, 24);
    A = R2(A,B,C,D,current_message[9], T, 5, 25);
    D = R2(D,A,B,C,current_message[14], T, 9, 26);
    C = R2(C,D,A,B,current_message[3], T, 14, 27);
    B = R2(B,C,D,A,current_message[8], T, 20, 28);
    A = R2(A,B,C,D,current_message[13], T, 5, 29);
    D = R2(D,A,B,C,current_message[2], T, 9, 30);
    C = R2(C,D,A,B,current_message[7], T, 14, 31);
    B = R2(B,C,D,A,current_message[12], T, 20, 32);

    // Round 3: 16 operations
    A = R3(A,B,C,D,current_message[5], T, 4,33);
    D = R3(D,A,B,C,current_message[8], T, 11,34);
    C = R3(C,D,A,B,current_message[11],T, 16,35);
    B = R3(B,C,D,A,current_message[14], T, 23,36);
    A = R3(A,B,C,D,current_message[1], T, 4,37);
    D = R3(D,A,B,C,current_message[4], T, 11,38);
    C = R3(C,D,A,B,current_message[7], T, 16,39);
    B = R3(B,C,D,A,current_message[10], T, 23,40);
    A = R3(A,B,C,D,current_message[13], T, 4,41);
    D = R3(D,A,B,C,current_message[0], T, 11,42);
    C = R3(C,D,A,B,current_message[3], T, 16,43);
    B = R3(B,C,D,A,current_message[6], T, 23,44);
    A = R3(A,B,C,D,current_message[9], T, 4,45);
    D = R3(D,A,B,C,current_message[12], T, 11,46);
    C = R3(C,D,A,B,current_message[15], T, 16,47);
    B = R3(B,C,D,A,current_message[2], T, 23,48);

    // Round 4: 16 operations
    A = R4(A,B,C,D,current_message[0], T, 6,49);
    D = R4(D,A,B,C,current_message[7], T, 10,50);
    C = R4(C,D,A,B,current_message[14], T, 15,51);
    B = R4(B,C,D,A,current_message[5], T, 21,52);
    A = R4(A,B,C,D,current_message[12], T, 6,53);
    D = R4(D,A,B,C,current_message[3], T, 10,54);
    C = R4(C,D,A,B,current_message[10], T, 15,55);
    B = R4(B,C,D,A,current_message[1], T, 21,56);
    A = R4(A,B,C,D,current_message[8], T, 6,57);
    D = R4(D,A,B,C,current_message[15], T, 10,58);
    C = R4(C,D,A,B,current_message[6], T, 15,59);
    B = R4(B,C,D,A,current_message[13], T, 21,60);
    A = R4(A,B,C,D,current_message[4], T, 6,61);
    D = R4(D,A,B,C,current_message[11], T, 10,62);
    C = R4(C,D,A,B,current_message[2], T, 15,63);
    B = R4(B,C,D,A,current_message[9], T, 21,64);

     // Last step:
     A = (A + AA) % __UINT32_MAX__;
     B = (B + BB) % __UINT32_MAX__;
     C = (C + CC) % __UINT32_MAX__;
     D = (D + DD) % __UINT32_MAX__;

    // Print test
    printf("A = %X\nB = %X\nC = %X\nD = %X\n", A, B, C, D);
}


// Give output hash 128 bits = 16 bytes
unsigned char* outputHash () {

}



// Input: Array of int (or char...)
// Output: Array of hexa char as md5 hash
unsigned char* hashmd5(unsigned char* data_buffer, unsigned int total_size) {
    //TODO 3
    
}


void main(int argc, char *argv[]) {
    unsigned char *file_buffer, *data_buffer;
    unsigned int *message_all_block, *current_message, *T;

    char input_file[] = "md5_data_test";
    char constant_t_file[] = "constant_t.txt";

    unsigned int i, j;
    unsigned int file_size, total_size, message_len; // 32 bits
    unsigned int A, B, C, D; // Buffer value

    // Buffer initialization
    A = 0x01234567;
    B = 0x89abcdef;
    C = 0xfedcba98;
    D = 0x76543210;


    // Determine file size (in bytes)
    file_size = findSize(input_file);
    if (file_size == -1) {
        printf("Problem while reading file.\n");
        exit(EXIT_FAILURE);
    }

    // Read file
    file_buffer = readBinary(input_file);

    // Calculate total data size (in bytes)
    total_size = getDataSize(file_buffer, file_size);
    message_len = total_size / 4; // Message len (in int32)

    // Padding
    data_buffer = padding(file_buffer, file_size, total_size);

    // Convert message to int
    message_all_block = convertUint(data_buffer, total_size, message_len);

    // Recover constant T
    T = getT(constant_t_file);

    // 
    // Main algorithm
    // Each block is 512 bits = 16 elements of message_all_block (each elem is 32 bits)
    // Output: concat(A, B, C, D)
    for (i=0; i<message_len; i+=BLOCK_SIZE_INT) {

        // Get the current block
        current_message = getCurrentBlock(message_all_block, i);

        // Process the current block
        processBlock(current_message, T, A, B, C, D);
    }



    // // Print test message
    // printf("File size: %u bytes\nPadded size: %u bytes = %u uint32\n", file_size, total_size, message_len);
    // for (i=0; i<total_size; i++) {
    //     printf("%4u", data_buffer[i]);
    // }
    // printf("\n");
    // for (j=0; j<message_len; j++) {
    //     printf("%u ", message_all_block[j]);
    // }
    // printf("\n");

    // // Print constant
    // for (i=0; i<64; i++) {
    //     printf("%X\n", T[i]);
    // }

    // Main MD5 functions
    hashmd5(data_buffer, total_size);
}