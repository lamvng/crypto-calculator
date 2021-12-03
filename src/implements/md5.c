#include "../libs/md5.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <math.h>


#define BLOCK_SIZE 64 // Block size for hashing = 512 bits = 64 bytes
#define LAST_BLOCK_SIZE 56 // If data of last block size = 56 bytes --> An entire new block must be add just for padding


int findSize(const char* file_name)
{
    struct stat st; /*declare stat variable*/

    /*get the size using stat()*/

    if (stat(file_name, &st) == 0)
        return (st.st_size);
    else 
        return -1;
}


// Read data from file
unsigned char* readBinary(char* filename) {

    int file_size;

    // Determine file size
    file_size = findSize(filename);
    if (file_size == -1) {
        printf("Problem while reading file.\n");
        exit(-1);
    }

    // Allocate buffer
    unsigned char* file_buffer = (unsigned char*) malloc(file_size * sizeof(unsigned char));
    printf("Buffer size: %u\n", sizeof(file_buffer) / sizeof(file_buffer[0]));
    // BUG: Buffer do not allocate file_size elements

    // Read file
    FILE *ptr;
    ptr = fopen(filename, "rb");
    fread(file_buffer, sizeof(file_buffer), 1, ptr);
    fclose(ptr);

    printf("File size: %u\n", file_size);

    return file_buffer;
}


// Padding data
unsigned char* padding(unsigned char* file_buffer) {

    int file_size, file_size_padding, num_block, size_last_block, total_size;
    long int file_size_padding_bit; // 64 bits

    // Determine file size (bytes)
    file_size = sizeof(file_buffer) / sizeof(file_buffer[0]);

    // Determine the number of necessary blocks
    num_block = (int) ceil ((double)file_size / BLOCK_SIZE);
    size_last_block = file_size % BLOCK_SIZE;

    // If data of last block size = 56 bytes --> An entire new block must be add just for padding
    // Or if size_last_block == 0 --> Add new block anyways
    if (size_last_block >= LAST_BLOCK_SIZE || size_last_block == 0) {
        num_block += 1;
    }

    // Get file size in 64 bits = 8 bytes
    // AND with 0xFF (2 F - since file_size_padding is in bytes)
    file_size_padding = file_size & 0xFF;

    // Allocate array to store data of all blocks
    total_size = num_block * BLOCK_SIZE;
    unsigned char data_buffer[total_size];

    // Pad data_buffer
    file_size_padding_bit = file_size_padding * 8;
    // TODO: Dich bit?






    // DONE: Read file as binary (char or int representation), SetBit ClearBit TestBit operations
    // TODO: Allocate buffer, padding


    return data_buffer;
}




// Input: Array of int (0 or 1)
// Output: Array of hexa char as md5 hash
char* hashmd5(char* inputBinString) {
    
}


void main(int argc, char *argv[]) {
    unsigned char *file_buffer, *data_buffer;
    char filename[] = "md5_data_test";


    // BUG: Function do not return the whole buffer, must use void()
    file_buffer = readBinary(filename);
    data_buffer = padding(file_buffer);


    // Print testing
    // Determine file size
    int file_size = findSize(filename);
    if (file_size == -1) {
        printf("Problem while reading file.\n");
        exit(-1);
    }
    for (int i=0; i<file_size; i++) {
        printf("%3u ", file_buffer[i]);
    }
    printf("\n");
    for (int i=0; i<file_size; i++) {
        printf("%3c ", file_buffer[i]);
    }
    printf("\n");



}