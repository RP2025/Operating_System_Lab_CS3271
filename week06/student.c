#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define NUM_CHAIRS 3

volatile sig_atomic_t chairs_available = NUM_CHAIRS;

void handle_sigint(int sig) {
    if (chairs_available < NUM_CHAIRS) {
        chairs_available++;
        printf("\nStudent left the classroom. Chair freed.\n");
    }
    exit(EXIT_SUCCESS);
}

void print_message() {
    printf("Please enter the classroom!\n");
}

int main() {
    signal(SIGINT, handle_sigint); // Register signal handler for Ctrl+C

    while (1) {
        if (chairs_available > 0) {
            print_message();
            chairs_available--;
            sleep(1); // Simulate student occupying chair
        } else {
            printf("No chair available. Waiting for a chair to become free...\n");
            while (chairs_available == 0) {} // Wait until a chair becomes available
        }
    }

    return 0;
}

