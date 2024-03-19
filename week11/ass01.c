#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 1024

typedef struct {
    int n; // Total number of blocks the file is storing
    int s; // Size of each block in bytes
    int f; // Number of free blocks
    char* fb; // Bit pattern showing free blocks
} Superblock;

int read_superblock(const char* fname, Superblock* sb) {
    FILE* fp = fopen(fname, "rb"); // Open for reading in binary mode
    if (fp == NULL) {
        perror("Error opening file");
        return 0;
    }
    fread(sb, sizeof(Superblock), 1, fp);
    fclose(fp);
    return 1;
}

int write_superblock(const char* fname, Superblock* sb) {
    FILE* fp = fopen(fname, "wb"); // Open for writing in binary mode
    if (fp == NULL) {
        perror("Error opening file");
        return 0;
    }
    fwrite(sb, sizeof(Superblock), 1, fp);
    fclose(fp);
    return 1;
}

int readblock(const char* fname, int ch_no, int bno, char* buffer) {
    FILE* fp = fopen(fname, "rb");
    if (fp == NULL) {
        perror("Error opening file");
        return 0;
    }

    // Move file pointer to the block to be read
    fseek(fp, sizeof(Superblock) + ch_no * sizeof(int) + bno * BLOCK_SIZE, SEEK_SET);

    // Read block data into buffer
    fread(buffer, BLOCK_SIZE, 1, fp);

    fclose(fp);
    return 1;
}

int writeblock(const char* fname, int ch_no, int bno, char* buffer) {
    FILE* fp = fopen(fname, "r+b");
    if (fp == NULL) {
        perror("Error opening file");
        return 0;
    }

    // Move file pointer to the block to be written
    fseek(fp, sizeof(Superblock) + ch_no * sizeof(int) + bno * BLOCK_SIZE, SEEK_SET);

    // Write block data from buffer
    fwrite(buffer, BLOCK_SIZE, 1, fp);

    fclose(fp);
    return 1;
}

int main() {
    // Initialize Superblock
    Superblock sb;
    sb.n = 100; // Example value, change accordingly
    sb.s = BLOCK_SIZE;
    sb.f = 100; // Example value, change accordingly
    sb.fb = (char*)malloc(sb.n / 8 * sizeof(char)); // Example allocation, change accordingly

    // Write Superblock to file
    if (!write_superblock("file.bin", &sb)) {
        fprintf(stderr, "Failed to write superblock\n");
        return EXIT_FAILURE;
    }

    // Read Superblock from file
    Superblock read_sb;
    if (!read_superblock("file.bin", &read_sb)) {
        fprintf(stderr, "Failed to read superblock\n");
        return EXIT_FAILURE;
    }

    // Display read Superblock values
    printf("Total number of blocks: %d\n", read_sb.n);
    printf("Size of each block: %d\n", read_sb.s);
    printf("Number of free blocks: %d\n", read_sb.f);

    // Example usage of readblock and writeblock
    char buffer[BLOCK_SIZE];
    int ch_no = 0;
    int bno = 0;

    // Read block
    if (!readblock("file.bin", ch_no, bno, buffer)) {
        fprintf(stderr, "Failed to read block\n");
        return EXIT_FAILURE;
    }

    // Write block (example)
    if (!writeblock("file.bin", ch_no, bno, buffer)) {
        fprintf(stderr, "Failed to write block\n");
        return EXIT_FAILURE;
    }

    // Free allocated memory for fb
    free(sb.fb);

    return EXIT_SUCCESS;
}

