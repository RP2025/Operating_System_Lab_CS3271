#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

// Define constants for data types
#define INT_TYPE    1
#define FLOAT_TYPE  2
#define DOUBLE_TYPE 3
#define CHAR_TYPE   4
#define STRING_TYPE 5

// Structure for encapsulating data
typedef struct {
    int type; // Type identifier
    union {
        int int_data;
        float float_data;
        double double_data;
        char char_data;
        char string_data[100]; // Assume max string length of 100
    } data;
} QueueData;

// Function prototypes
void enqueue(int pipefd[2], QueueData data);
QueueData dequeue(int pipefd[2]);

int main() {
    int pipefds[2]; // File descriptors for the pipe
    int flag; // Return value of system calls
    QueueData data; // Data to be enqueued/dequeued

    // Create the pipe
    flag = pipe(pipefds);
    if (flag == -1) {
        perror("pipe() failed:");
        exit(EXIT_FAILURE);
    }

    // Enqueue integer data
    data.type = INT_TYPE;
    data.data.int_data = 42;
    enqueue(pipefds, data);

    // Enqueue float data
    data.type = FLOAT_TYPE;
    data.data.float_data = 3.14;
    enqueue(pipefds, data);

    // Enqueue string data
    data.type = STRING_TYPE;
    strcpy(data.data.string_data, "Hello, world!");
    enqueue(pipefds, data);

    // Dequeue data and print
    for (int i = 0; i < 3; i++) {
        data = dequeue(pipefds);
        switch (data.type) {
            case INT_TYPE:
                printf("Dequeued integer: %d\n", data.data.int_data);
                break;
            case FLOAT_TYPE:
                printf("Dequeued float: %f\n", data.data.float_data);
                break;
            case STRING_TYPE:
                printf("Dequeued string: %s\n", data.data.string_data);
                break;
            default:
                printf("Unknown data type\n");
                break;
        }
    }

    return 0;
}

// Enqueue data into the pipe
void enqueue(int pipefd[2], QueueData data) {
    int flag;
    flag = write(pipefd[1], &data, sizeof(QueueData));
    if (flag == -1) {
        perror("write() failed:");
        exit(EXIT_FAILURE);
    }
}

// Dequeue data from the pipe
QueueData dequeue(int pipefd[2]) {
    int flag;
    QueueData data;
    flag = read(pipefd[0], &data, sizeof(QueueData));
    if (flag == -1) {
        perror("read() failed:");
        exit(EXIT_FAILURE);
    }
    return data;
}

