#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_SIZE 1024 // Default size in bytes for the dd1 file

int main() {
    const char *filename = "dd1";
    FILE *file = fopen(filename, "wb"); // Open file for writing in binary mode

    if (file == NULL) {
        perror("Error creating file");
        return 1;
    }

    // Set the file size to DEFAULT_SIZE bytes
    if (fseek(file, DEFAULT_SIZE - 1, SEEK_SET) != 0) {
        perror("Error setting file size");
        fclose(file);
        return 1;
    }

    // Write a single byte to the end of the file to ensure its size is set
    if (fwrite("", 1, 1, file) != 1) {
        perror("Error writing to file");
        fclose(file);
        return 1;
    }

    fclose(file); // Close the file
    printf("File '%s' created successfully with size %d bytes.\n", filename, DEFAULT_SIZE);

    return 0;
}

