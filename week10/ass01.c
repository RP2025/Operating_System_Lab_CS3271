#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INFO_SIZE 1024

// Function to initialize a file with specified number of blocks and block size
int init_File_dd(const char *fname, int bsize, int bno) {
    // Open the file in binary write mode
    FILE *file = fopen(fname, "wb");
    if (file == NULL) {
        perror("Error creating file");
        return -1;
    }

    // Write the file information (number of blocks and block size) to the beginning of the file
    int info[2] = {bno, bsize};
    fwrite(info, sizeof(int), 2, file);

    // Seek to the end of the file to allocate space for blocks
    fseek(file, INFO_SIZE + bsize * bno - 1, SEEK_SET);
    fputc('\0', file); // Write a null byte to allocate space

    // Close the file
    fclose(file);

    return 0;
}

// Function to read a block from the file
int read_block(const char *fname, int bno, char *buffer) {
    // Open the file in binary read mode
    FILE *file = fopen(fname, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return 0;
    }

    // Read the file information (number of blocks and block size) from the beginning of the file
    int info[2];
    fread(info, sizeof(int), 2, file);
    int bsize = info[1];

    // Seek to the start of the requested block
    fseek(file, INFO_SIZE + bno * bsize, SEEK_SET);

    // Read the block into the buffer
    fread(buffer, 1, bsize, file);

    // Close the file
    fclose(file);

    return 1;
}

// Function to write a block to the file
int write_block(const char *fname, int bno, char *buffer) {
    // Open the file in binary read-write mode
    FILE *file = fopen(fname, "rb+");
    if (file == NULL) {
        perror("Error opening file");
        return 0;
    }

    // Read the file information (number of blocks and block size) from the beginning of the file
    int info[2];
    fread(info, sizeof(int), 2, file);
    int bsize = info[1];

    // Seek to the start of the requested block
    fseek(file, INFO_SIZE + bno * bsize, SEEK_SET);

    // Write the block from the buffer to the file
    fwrite(buffer, 1, bsize, file);

    // Close the file
    fclose(file);

    return 1;
}

int main() {
    const char *fname = "dd1";
    int bsize = 4096; // Block size
    int bno = 2048;   // Number of blocks

    // Initialize the file
    if (init_File_dd(fname, bsize, bno) == -1) {
        printf("Failed to initialize the file.\n");
        return 1;
    }

    // Test reading and writing blocks
    char buffer[bsize];
    memset(buffer, 'A', bsize); // Fill buffer with 'A's

    // Write block 0
    if (write_block(fname, 0, buffer) == 1) {
        printf("Block 0 written successfully.\n");
    } else {
        printf("Failed to write block 0.\n");
        return 1;
    }

    // Read block 0
    char read_buffer[bsize];
    if (read_block(fname, 0, read_buffer) == 1) {
        printf("Block 0 read successfully.\n");
        printf("Contents of block 0: %s\n", read_buffer);
    } else {
        printf("Failed to read block 0.\n");
        return 1;
    }

    return 0;
}

