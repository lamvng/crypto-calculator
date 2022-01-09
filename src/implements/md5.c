#include "../libs/md5.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <math.h>


#define BLOCK_SIZE 64 // Block size for hashing = 512 bits = 64 bytes
#define LAST_BLOCK_SIZE 56 // If data of last block size = 56 bytes --> An entire new block must be add just for padding


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
unsigned char* readBinary(char* filename) {

    unsigned int file_size;

    // Determine file size
    file_size = findSize(filename);
    if (file_size == -1) {
        printf("Problem while reading file.\n");
        exit(-1);
    }

    // Allocate buffer
    unsigned char* file_buffer = (unsigned char*) malloc(file_size * sizeof(unsigned char));
    // BUG: Buffer do not allocate file_size elements

    // Read file
    FILE *ptr;
    ptr = fopen(filename, "rb");
    fread(file_buffer, sizeof(unsigned char), file_size, ptr);
    fclose(ptr);

    return file_buffer;
}


// Calculate total data size after padding (in bytes)
unsigned int getDataSize(unsigned char* file_buffer, unsigned int file_size) {
    unsigned int file_size_padding, num_block, size_last_block, total_size;
    long unsigned int file_size_padding_bit; // 64 bits

    // Determine the number of necessary blocks
    num_block = (unsigned int) ceil ((double)file_size / BLOCK_SIZE);
    size_last_block = file_size % BLOCK_SIZE;

    // If data of last block size = 56 bytes --> An entire new block must be add just for padding
    // Or if size_last_block == 0 --> Add new block anyways
    if (size_last_block >= LAST_BLOCK_SIZE || size_last_block == 0) {
        num_block += 1;
    }

    total_size = num_block * BLOCK_SIZE;

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
        exit(-1);
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
    printf("\n");

    return data_buffer;
}

//TODO
// Initialize buffer of 128 bits = 16 bytes --> Output hash
// 16 bytes --> 4 words of A B C D, each 4 bytes
unsigned char* initBuffer() {

}


// 4 functions F G H I to manipulate the buffer

// F (B, C, D) = (B & C) | (~B & D)
unsigned char* F (unsigned char* B, unsigned char* C, unsigned char* D) {
    return (unsigned char*)(((int)B & (int)C) | (~(int)B & (int)D));
}

// G (B, C, D) = (B & D) | (C & ~D)
unsigned char* G (unsigned char* B, unsigned char* C, unsigned char* D) {
    return (unsigned char*)(((int)B & (int)D) | ((int)C & ~(int)D));
}

// H (B, C, D) = B XOR C XOR D
unsigned char* H (unsigned char* B, unsigned char* C, unsigned char* D) {
    return (unsigned char*)((int)B ^ (int)C ^ (int)D);
}

// I (B, C, D) = C XOR (B | ~D)
unsigned char* I (unsigned char* B, unsigned char* C, unsigned char* D) {
    return (unsigned char*)((int)C ^ ((int)B | ~(int)D));
}




//TODO
// Process each block of 512 bits = 64 bytes
void processBlock (unsigned char* block) {

}


// Give output hash 128 bits = 16 bytes
unsigned char* outputHash () {

}



// Input: Array of int (0 or 1)
// Output: Array of hexa char as md5 hash
unsigned char* hashmd5(unsigned char* data_buffer, unsigned int total_size) {
    //TODO: Split data_buffer into block, each block is splitted into messages

}


void main(int argc, char *argv[]) {
    unsigned char *file_buffer, *data_buffer;
    char filename[] = "md5_data_test";
    unsigned int i, j;
    unsigned int file_size, total_size; // 32 bits
    int A, B, C, D; // Buffer value

    // Buffer initialization
    A = 0x01234567;
    B = 0x89abcdef;
    C = 0xfedcba98;
    D = 0x76543210;


    // Determine file size
    file_size = findSize(filename);
    if (file_size == -1) {
        printf("Problem while reading file.\n");
        exit(-1);
    }

    // Read file
    file_buffer = readBinary(filename);

    // Calculate total data size
    total_size = getDataSize(file_buffer, file_size);


    // Padding
    data_buffer = padding(file_buffer, file_size, total_size);


    // Print testing
    for (i=0; i<total_size; i++) {
        printf("%4u", data_buffer[i]);
    }
    printf("\n");


    // Main MD5 functions
    hashmd5(data_buffer, total_size);
}
