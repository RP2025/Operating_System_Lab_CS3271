#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define BUFFER_SIZE 100 // Maximum size of data to be enqueued

// Structure for encapsulating data
typedef struct {
    int data_type;      // Type identifier
    void *data;         // Pointer to the data
    size_t data_size;   // Size of the data in bytes
} QueueData;

// Function prototypes
void enqueue(int write_fd, QueueData data);
QueueData dequeue(int read_fd);

int main() {
    int pipefds[2]; // File descriptors for the pipe
    int flag;       // Return value of system calls
    pid_t pid;      // Process ID of the child process
    QueueData data; // Data to be enqueued

    // Create the pipe
    flag = pipe(pipefds);
    if (flag == -1) {
        perror("pipe() failed:");
        exit(EXIT_FAILURE);
    }

    // Fork a child process
    pid = fork();
    if (pid == -1) {
        perror("fork() failed:");
        exit(EXIT_FAILURE);
    } else if (pid == 0) { // Child process (P2)
        close(pipefds[1]); // Close the write end of the pipe
        printf("Child process (P2) is dequeuing data from the queue...\n");

        // Dequeue data and print
        while (1) {
            data = dequeue(pipefds[0]);
            if (data.data_type == -1) { // Exit condition
                break;
            }
            printf("Dequeued data of type %d\n", data.data_type);
            // Here, you can add code to handle the dequeued data according to its type
            // For now, we just print the type identifier
        }

        close(pipefds[0]); // Close the read end of the pipe
        printf("Child process (P2) has finished dequeuing data.\n");
    } else { // Parent process (P1)
        close(pipefds[0]); // Close the read end of the pipe
        printf("Parent process (P1) is enqueuing data into the queue...\n");

        // Enqueue data of different types
        data.data_type = 1;
        int int_data = 42;
        enqueue(pipefds[1], (QueueData){.data_type = 1, .data = &int_data, .data_size = sizeof(int)});

        data.data_type = 2;
        float float_data = 3.14;
        enqueue(pipefds[1], (QueueData){.data_type = 2, .data = &float_data, .data_size = sizeof(float)});

        data.data_type = 3;
        char char_data = 'A';
        enqueue(pipefds[1], (QueueData){.data_type = 3, .data = &char_data, .data_size = sizeof(char)});

        // Send termination signal
        enqueue(pipefds[1], (QueueData){.data_type = -1, .data = NULL, .data_size = 0});

        close(pipefds[1]); // Close the write end of the pipe
        printf("Parent process (P1) has finished enqueuing data.\n");
    }

    return 0;
}

// Enqueue data into the pipe
void enqueue(int write_fd, QueueData data) {
    int flag;
    flag = write(write_fd, &data, sizeof(QueueData));
    if (flag == -1) {
        perror("write() failed:");
        exit(EXIT_FAILURE);
    }
}

// Dequeue data from the pipe
QueueData dequeue(int read_fd) {
    int flag;
    QueueData data;
    flag = read(read_fd, &data, sizeof(QueueData));
    if (flag == -1) {
        perror("read() failed:");
        exit(EXIT_FAILURE);
    }
    return data;
}

