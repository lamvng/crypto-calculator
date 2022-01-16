#include "../libs/md5.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <math.h>
#include <stdint.h>


#define BLOCK_SIZE_BYTE 64 // Block size for hashing = 512 bits = 64 bytes
#define LAST_BLOCK_SIZE_BYTE 56 // If data of last block size = 56 bytes --> An entire new block must be add just for padding
#define BLOCK_SIZE_INT (BLOCK_SIZE_BYTE / 4)
#define NUM_BITS_IN_INT (sizeof(int) * 8)

// #define F(X, Y, Z) ((X & Y) | (~X & Z))
// #define G(X, Y, Z) ((X & Z) | (Y & ~Z))
// #define H(X, Y, Z) (X ^ Y ^ Z)
// #define I(X, Y, Z) (Y ^ (X | ~Z))



// Website to check for padding
// https://fthb321.github.io/MD5-Hash/MD5OurVersion2.html





// Assign value to a bit position k in an array
// http://www.mathcs.emory.edu/%7Echeung/Courses/255/Syllabus/1-C-intro/bit-array.html
void toggleBit(unsigned char* array, uint32_t value, unsigned int k) {
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
unsigned int getByte(uint64_t number, unsigned int no_byte) {
    return (number >> (no_byte * 8)) & 0xFF;    // Mask and return the required byte
}


// Read data from file
unsigned char* readBinary(char* input_file) {

    unsigned int file_size;

    // Deterine file size
    file_size = findSize(input_file);
    if (file_size == -1) {
        printf("Problem while reading file.\n");
        exit(EXIT_FAILURE);
    }

    // Allocate buffer
    unsigned char* file_buffer = (unsigned char*) malloc(file_size * sizeof(unsigned char));


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
    uint64_t file_size_padding_bit;

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
    // FIXME: Problem with data len padding
    file_size_padding_bit = file_size_padding << 3; // Data size in bit

    // Append each (len) byte from left to right, from LSB to MSB
    // file_size_padding_bit is 64 bits, hence byte_position (MSB) is 8
    unsigned int byte_position = 0;
    for (i=total_size-8; i<total_size; i++) {
        data_buffer[i] = getByte(file_size_padding_bit, byte_position);
        byte_position ++;
    }

    return data_buffer;
}


// Convert char* data_buffer array to int* buffer array after padding
// For later calculation on int
uint32_t* convertUint (unsigned char* data_buffer, unsigned int total_size, unsigned int message_len_int32) {
    uint32_t* message_all_block = (uint32_t*) malloc(message_len_int32 * sizeof(uint32_t));
    int i,j;
    for (i=0; i<total_size; i+=4){
        // Left shift to append hex value
        // message_all_block[j] = ((uint32_t)data_buffer[i] << 24) | ((uint32_t)data_buffer[i+1] << 16) | ((uint32_t)data_buffer[i+2] << 8) | ((uint32_t)data_buffer[i+3]);
        message_all_block[j] = ((uint32_t)data_buffer[i]) | ((uint32_t)data_buffer[i+1] << 8) | ((uint32_t)data_buffer[i+2] << 16) | ((uint32_t)data_buffer[i+3] << 24);

        j++;
    }
    return message_all_block;
}


// 4 functions F G H I

// F (X, Y, Z) = (X & Y) | (~X & Z)
uint32_t F (uint32_t X, uint32_t Y, uint32_t Z) {
    return ((X & Y) | (~X & Z));
}


// G (X, Y, Z) = (X & Z) | (Y & ~Z)
uint32_t G (uint32_t X, uint32_t Y, uint32_t Z) {
    return ((X & Z) | (Y & ~Z));
}

// H (X, Y, Z) = X XOR Y XOR Z
uint32_t H (uint32_t X, uint32_t Y, uint32_t Z) {
    return (X ^ Y ^ Z);
}

// I (X, Y, Z) = Y XOR (X | ~Z)
uint32_t I (uint32_t X, uint32_t Y, uint32_t Z) {
    return (Y ^ (X | ~Z));
}


// Circular left shift in uint32_t variable
// https://stackoverflow.com/questions/7162454/circular-shifting-in-c
uint32_t rotleft(uint32_t num, uint32_t shift) {
    return (num << shift) | (num >> (NUM_BITS_IN_INT - shift));
}


// Save buffer
void *saveBuffer(uint32_t* buffer, uint32_t A, uint32_t B, uint32_t C, uint32_t D) {
    buffer[0] = A;
    buffer[1] = B;
    buffer[2] = C;
    buffer[3] = D;
}


// 4 Rounds function
// Divide the current block into 16 words --> No need to, message already has 16 elements of uint32
// return a
// a b c d: Buffer
// X : Message (32 bits) in block (512 bits)
// T[i] : Constant T, index i (T[i] = T[1..64])

// a = b + ((a + F(b,c,d) + X + T[i]) <<< s)
uint32_t R1(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t X, uint32_t* T, unsigned int s, unsigned int i) {
    return b + rotleft((a + F(b, c, d) + X + T[i-1]), s);
}

// a = b + ((a + G(b,c,d) + X + T[i]) <<< s)
uint32_t R2(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t X, uint32_t* T, unsigned int s, unsigned int i) {
    return b + rotleft((a + G(b, c, d) + X + T[i-1]), s);
}

// a = b + ((a + H(b,c,d) + X + T[i]) <<< s)
uint32_t R3(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t X, uint32_t* T, unsigned int s, unsigned int i) {
    return b + rotleft((a + H(b, c, d) + X + T[i-1]), s);
}

// a = b + ((a + I(b,c,d) + X + T[i]) <<< s)
uint32_t R4(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t X, uint32_t* T, unsigned int s, unsigned int i) {
    return b + rotleft((a + I(b, c, d) + X + T[i-1]), s);
}


// Calculate constant T and write to file
void calculateT(char* constant_t_file) {

    uint32_t val;

    // Constant T file
    FILE *ptr;
    ptr = fopen(constant_t_file, "w");
    if (ptr == NULL) {
        printf("Error opening the file %s", constant_t_file);
        exit(EXIT_FAILURE);
    }

    for (int i=1; i<=64; i++) {
        // 0x100000000 = 2^32
        val = (uint32_t) (fabs(sin((double) i)) * 0x100000000);
        fprintf(ptr, "%X\n", val);
    }

    fclose(ptr);
}


// Load constant T (pre-calculated) from file
// T[1, 2, ..., 64]
// T[i] = int(abs(sin(i)) * 2**32)
uint32_t* getT(char* constant_t_file) {
    // Read file
    FILE *ptr;
    char* line = NULL;
    size_t len_line = 0;
    ssize_t read_line;
    uint32_t *T;
    int i = 0;
    T = (uint32_t*) malloc(64 * sizeof(uint32_t));

    ptr = fopen(constant_t_file, "r");
    if(ptr == NULL) {
        printf("Unable to open file!\n");
        exit(EXIT_FAILURE);
    }
    while ((read_line = getline(&line, &len_line, ptr)) != -1) {
        sscanf(line, "%X", &T[i]);
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
uint32_t* getCurrentBlock(uint32_t* message_all_block, unsigned int i) {
    uint32_t* current_message = (uint32_t*) malloc(BLOCK_SIZE_INT * sizeof(uint32_t));
    for (int j=0; j<BLOCK_SIZE_INT; j++) {
        current_message[j] = message_all_block[i];
        i++;
    }
    return current_message;
}


// http://herongyang.com/Cryptography/MD5-Message-Digest-Algorithm-Overview.html
// Process each block of 512 bits = 16 elements of message_all_block
// A B C D: Buffer
// X : Message (32 bits) in block (512 bits)
// T[i] : Constant T, index i
// NOTE // Attention: In the algo, T[i] is T[1..64] --> Need conversion to T[0..63]
void md5Block (uint32_t* current_message, uint32_t* T, uint32_t* buffer) {
    uint32_t A, B, C, D, AA, BB, CC, DD;

    // Init
    A = buffer[0];
    B = buffer[1];
    C = buffer[2];
    D = buffer[3];

    AA = A;
    BB = B;
    CC = C;
    DD = D;

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

    // // Print round 1 result
    // printf("\nFunction F results:");
    // printf("\nA = %x\nB = %x\nC = %x\nD = %x\n", A, B, C, D);

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

    // // Print round 2 result
    // printf("\nFunction G results:");
    // printf("\nA = %x\nB = %x\nC = %x\nD = %x\n", A, B, C, D);

    // Round 3: 16 operations
    A = R3(A,B,C,D,current_message[5], T, 4, 33);
    D = R3(D,A,B,C,current_message[8], T, 11, 34);
    C = R3(C,D,A,B,current_message[11],T, 16, 35);
    B = R3(B,C,D,A,current_message[14], T, 23, 36);
    A = R3(A,B,C,D,current_message[1], T, 4, 37);
    D = R3(D,A,B,C,current_message[4], T, 11, 38);
    C = R3(C,D,A,B,current_message[7], T, 16, 39);
    B = R3(B,C,D,A,current_message[10], T, 23, 40);
    A = R3(A,B,C,D,current_message[13], T, 4, 41);
    D = R3(D,A,B,C,current_message[0], T, 11, 42);
    C = R3(C,D,A,B,current_message[3], T, 16, 43);
    B = R3(B,C,D,A,current_message[6], T, 23, 44);
    A = R3(A,B,C,D,current_message[9], T, 4, 45);
    D = R3(D,A,B,C,current_message[12], T, 11, 46);
    C = R3(C,D,A,B,current_message[15], T, 16, 47);
    B = R3(B,C,D,A,current_message[2], T, 23, 48);

    // // Print round 2 result
    // printf("\nFunction H results:");
    // printf("\nA = %x\nB = %x\nC = %x\nD = %x\n", A, B, C, D);

    // Round 4: 16 operations
    A = R4(A,B,C,D,current_message[0], T, 6, 49);
    D = R4(D,A,B,C,current_message[7], T, 10, 50);
    C = R4(C,D,A,B,current_message[14], T, 15, 51);
    B = R4(B,C,D,A,current_message[5], T, 21, 52);
    A = R4(A,B,C,D,current_message[12], T, 6, 53);
    D = R4(D,A,B,C,current_message[3], T, 10, 54);
    C = R4(C,D,A,B,current_message[10], T, 15, 55);
    B = R4(B,C,D,A,current_message[1], T, 21, 56);
    A = R4(A,B,C,D,current_message[8], T, 6, 57);
    D = R4(D,A,B,C,current_message[15], T, 10, 58);
    C = R4(C,D,A,B,current_message[6], T, 15, 59);
    B = R4(B,C,D,A,current_message[13], T, 21, 60);
    A = R4(A,B,C,D,current_message[4], T, 6, 61);
    D = R4(D,A,B,C,current_message[11], T, 10, 62);
    C = R4(C,D,A,B,current_message[2], T, 15, 63);
    B = R4(B,C,D,A,current_message[9], T, 21, 64);

    // // Print round 4 result
    // printf("\nFunction I results:");
    // printf("\nA = %x\nB = %x\nC = %x\nD = %x\n", A, B, C, D);

    // Last step
    A = (A + AA);
    B = (B + BB);
    C = (C + CC);
    D = (D + DD);

    // Save buffer
    saveBuffer(buffer, A, B, C, D);

    // printf("\nBlock results:\nA = %x\nB = %x\nC = %x\nD = %x\n\n", A, B, C, D);
}


// Provide output hash from A B C D
void getOutputHash (unsigned char* output_hash, uint32_t A, uint32_t B, uint32_t C, uint32_t D) {
    // num & 0xFF000000; // MSB byte - 1st byte
    // num & 0x00FF0000; // MSB byte - 2nd byte
    // num & 0x0000FF00; // MSB byte - 3rd byte
    // num & 0x000000FF; // MSB byte - 4th byte

    output_hash[0] = (A & 0x000000FF);
    output_hash[1] = (A & 0x0000FF00) >> 8;
    output_hash[2] = (A & 0x00FF0000) >> 16;
    output_hash[3] = (A & 0xFF000000) >> 24;
    output_hash[4] = (B & 0x000000FF);
    output_hash[5] = (B & 0x0000FF00) >> 8;
    output_hash[6] = (B & 0x00FF0000) >> 16;
    output_hash[7] = (B & 0xFF000000) >> 24;
    output_hash[8] = (C & 0x000000FF);
    output_hash[9] = (C & 0x0000FF00) >> 8;
    output_hash[10] = (C & 0x00FF0000) >> 16;
    output_hash[11] = (C & 0xFF000000) >> 24;
    output_hash[12] = (D & 0x000000FF);
    output_hash[13] = (D & 0x0000FF00) >> 8;
    output_hash[14] = (D & 0x00FF0000) >> 16;
    output_hash[15] = (D & 0xFF000000) >> 24;
}    



// Input: String array (unsigned char)
// Output: Array of unsigned char as md5 hash
unsigned char* hashmd5(unsigned char* file_buffer) {
    unsigned char *data_buffer;
    uint32_t *message_all_block, *current_message, *T, *buffer;

    unsigned char* output_hash;
    output_hash = (unsigned char*) malloc(16 * sizeof(unsigned char*));

    char constant_t_file[] = "constant_t_md5.txt";

    unsigned int i, j;
    unsigned int file_size, total_size, message_len_int32; // 32 bits
    uint32_t A, B, C, D; // Buffer value

    // Buffer initialization

    A = 0x67452301;
    B = 0xEFCDAB89;
    C = 0x98BADCFE;
    D = 0x10325476;


    buffer = (uint32_t*) malloc(4 * sizeof(uint32_t));
    saveBuffer(buffer, A, B, C, D);


    // // TEST read input string from file
    // // Read file
    // unsigned char *file_buffer;
    // file_buffer = readBinary(input_file);
    // file_size = findSize(input_file);
    // if (file_size == -1) {
    //     printf("Problem while reading file.\n");
    //     exit(EXIT_FAILURE);
    // }

    // Example input string
    // unsigned char file_buffer[] = "They are deterministic";
    file_size = strlen(file_buffer);
    for (i=0; i<file_size; i++) {
        printf("%c", file_buffer[i]);
    }
    printf("\n\n");


    // Calculate total data size (in bytes)
    total_size = getDataSize(file_buffer, file_size);
    message_len_int32 = total_size / 4; // Message len (in int32)

    // Padding
    data_buffer = padding(file_buffer, file_size, total_size);

    // Convert message to int
    message_all_block = convertUint(data_buffer, total_size, message_len_int32);

    // Recover constant T
    // calculateT("constant_t_md5.txt");
    T = getT(constant_t_file);


    // Main algorithm
    // Each block is 512 bits = 16 elements of message_all_block (each elem is 32 bits)
    // Output: concat(A, B, C, D)
    for (i=0; i<message_len_int32; i+=BLOCK_SIZE_INT) {

        // Get the current block
        current_message = getCurrentBlock(message_all_block, i);

        // // Print current block
        // printf("Current block:\n");
        // for (j=0; j<BLOCK_SIZE_INT; j++) {
        // printf("%x ", current_message[j]);
        // }
        // printf("\n");

        // Process the current block
        md5Block(current_message, T, buffer);
    }

    // Load output
    A = buffer[0];
    B = buffer[1];
    C = buffer[2];
    D = buffer[3];

    // Get output hash (the little-endian way)
    getOutputHash(output_hash, A, B, C, D);

    // Final hash
    printf("Final MD5 hash:\n");
    for (i = 0; i<16; i++) {
        printf("%02x", output_hash[i]);
    }
    printf("\n\n");

    // Data array in char
    printf("\n");
    printf("File size: %u bytes\nPadded size: %u bytes = %u uint32\n", file_size, total_size, message_len_int32);
    printf("\nMessage bytes:\n");
    for (i=0; i<total_size; i++) {
        printf("%4x", data_buffer[i]);
    }
    printf("\n\n");

    // Data array in int32
    printf("Message in word (32 bits):\n");
    for (j=0; j<message_len_int32; j++) {
        printf("%x ", message_all_block[j]);
    }
    printf("\n");

    // // Print test circular left shift
    // uint32_t rot_num = 0x01234567;
    // unsigned int left_shift = 12;
    // printf("\nTestrotleft:\nrotleft by %u bits of %#x: %#x\n", left_shift, rot_num, rotleft(rot_num, left_shift));


    // // Constant T
    // printf("\nConstant T: \n");
    // for (i=0; i<64; i++) {
    //     printf("%x\n", T[i]);
    // }

    // Result
    // printf("\nFinal results:\n");
    // printf("A = %x\nB = %x\nC = %x\nD = %x\n", A, B, C, D);

    return output_hash;
}


// void main(int argc, char *argv[]) {
//     unsigned char* output_hash;
//     unsigned int i;

//     unsigned char file_buffer[] = "They are deterministic";

//     output_hash = hashmd5(file_buffer);

//     // Final hash
//     printf("Final MD5 hash:\n");
//     for (i = 0; i<16; i++) {
//         printf("%02x", output_hash[i]);
//     }
//     printf("\n\n");

// }