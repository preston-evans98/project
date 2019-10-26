#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "matrix.h"

int jobsSent = 0;
int jobsReceived = 0;

void sigIntHandler(int signum) {
    if (signum == SIGINT) {
        printf("Jobs sent: %d. Jobs received: %d", jobsSent, jobsReceived);
    } 
}

// void sigQuitHandler(int signum) {
//     if (signum == SIGQUIT){
//         printf("Signal is: %d\n", signum);
//     }
// }
int main(int argc, char** argv) {
    // Handle interrupts
    signal(SIGINT, sigIntHandler);
    // Check for correct number of arguments
    if (argc < 5) {
        printf("Usage: ./package <matrix 1 file> <matrix 2 file> <output matrix data file> <secs between thread creation>\n");
        exit(0);
    }
    // Parse args and verify that all file paths can be opened
    FILE *inputOne;
    FILE *inputTwo;
    FILE* output;
    int secs_between_threads;
    inputOne = fopen(argv[1], "r");
    if (inputOne == NULL) {
        printf("Could not open '%s' - aborting.\n", argv[1]);
    }
    inputTwo = fopen(argv[2], "r");
    if (inputTwo == NULL) {
        printf("Could not open '%s' - aborting.\n", argv[2]);
    }
    output = fopen(argv[3], "w");
    if (output == NULL) {
        printf("Could not open '%s' - aborting.\n", argv[3]);
    }
    secs_between_threads = atoi(argv[4]);

    // Read data from files
    Matrix* first = readMatrixFromFile(inputOne);
    Matrix* second = readMatrixFromFile(inputTwo);
    if (first->cols != second->rows) {
        printf("Error: Cannot multiply matrices with dimensions %dx%d, %dx%d", first->rows, first->cols, second->rows, second->cols);
    }



    return 0;
}
