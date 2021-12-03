#include "../libs/md5.h"
#include <stdio.h>
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


unsigned char* readBinary(char* filename) {

    int file_size, num_block, size_last_block;

    // Determine file size
    file_size = findSize(filename);
    if (file_size == -1) {
        printf("Problem while reading file.\n");
        exit(-1);
    }

    // Determine the number of necessary blocks
    num_block = (int) ceil ((double)file_size / BLOCK_SIZE);
    size_last_block = file_size % BLOCK_SIZE;

    // If data of last block size = 56 bytes --> An entire new block must be add just for padding
    


    // Array to store data
    unsigned char data_buffer[file_size];


    FILE *ptr;
    ptr = fopen(filename, "rb");
    fread(data_buffer, sizeof(data_buffer), 1, ptr);


    for (int i=0; i<file_size; i++) {
        printf("%3u ", data_buffer[i]);
    }
    printf("\n");
    for (int i=0; i<file_size; i++) {
        printf("%3c ", data_buffer[i]);
    }
    printf("\n");
    fclose(ptr);

    // DONE: Read file as binary (char or int representation), SetBit ClearBit TestBit operations
    // TODO: Allocate buffer, padding


    return data_buffer;
}




// Input: Array of int (0 or 1)
// Output: Array of hexa char as md5 hash
char* hashmd5(char* inputBinString) {
    
}


void main(int argc, char *argv[]) {
    char filename[] = "md5_data_test";
    readBinary(filename);
}