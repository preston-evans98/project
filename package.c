#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include "matrix.h"

typedef struct __payload_ {
    Matrix* first;
    Matrix* second;
    int sleepTime;
} Payload;

int jobsSent = 0;
int jobsReceived = 0;

void sigIntHandler(int signum) {
    if (signum == SIGINT) {
        printf("Jobs sent: %d. Jobs received: %d", jobsSent, jobsReceived);
    } 
}

void* packageSubtasks(void* payload) {
    Payload* data = (Payload*) payload;
    Matrix* first = data->first;
    Matrix* second = data->second;
    int sleeptTime = data->sleepTime;
    int numProducts = first->rows + second->cols;
    if (first == NULL || second == NULL) {
        fprintf(stderr, "Error: Null matrix passed to packageSubtasks\n");
        exit(0);
    }
    key_t key = 11793506;
    int msqid = msgget(key, 0666 | IPC_CREAT);
    if (msqid == -1) {
        fprintf(stderr, "Error: could not establish message queue.\n");
        // exit(-1);
    }
    int k = 0;
    int currentIndex = 0;
    int innerDim = first->cols;
    Msg messages[numProducts];
    
    for (int i = 0; i < first->rows; i++) {
        for (int j = 0; j < second->cols; j++) {
            messages[k].type = 1;
            messages[k].jobid = k;
            messages[k].rowvec = i;
            messages[k].colvec = j;
            messages[k].innerDim = innerDim;
            for (currentIndex = 0; currentIndex < innerDim; currentIndex++) {
                messages[k].data[currentIndex] = first->values[i][currentIndex];
                messages[k].data[currentIndex + innerDim] = second->values[currentIndex][j];
            }
            int rc = msgsnd(msqid, &messages[k], 2*innerDim, 0);
            printf("Sending job id %d type %d size %d (rc=%d)\n", k, 1, innerDim*2, rc);
            fflush(stdout);
            sleep(sleeptTime);
            k++;
        }
    }
    return NULL;
}

int main(int argc, char** argv) {
    // Handle interrupts
    signal(SIGINT, sigIntHandler);
    // Check for correct number of arguments
    if (argc < 5) {
        fprintf(stderr, "Usage: ./package <matrix 1 file> <matrix 2 file> <output matrix data file> <secs between thread creation>\n");
        exit(0);
    }
    // Parse args and verify that all file paths can be opened
    FILE *inputOne;
    FILE *inputTwo;
    FILE* output;
    int secsBetweenThreads;
    inputOne = fopen(argv[1], "r");
    if (inputOne == NULL) {
        fprintf(stderr, "Could not open '%s' - aborting.\n", argv[1]);
    }
    inputTwo = fopen(argv[2], "r");
    if (inputTwo == NULL) {
        fprintf(stderr, "Could not open '%s' - aborting.\n", argv[2]);
    }
    output = fopen(argv[3], "w");
    if (output == NULL) {
        fprintf(stderr, "Could not open '%s' - aborting.\n", argv[3]);
    }
    secsBetweenThreads = atoi(argv[4]);

    // Read data from files
    Matrix* first = readMatrixFromFile(inputOne);
    Matrix* second = readMatrixFromFile(inputTwo);
    if (first->cols != second->rows) {
        fprintf(stderr, "Error: Cannot multiply matrices with dimensions %dx%d, %dx%d", first->rows, first->cols, second->rows, second->cols);
    }
    #if DEBUG 
    printMatrix(first);
    printf("----------------------\n");
    printMatrix(second);
    #endif

    Payload* payload  = safeMalloc(sizeof(Payload));
    payload->first = first;
    payload->second = second;
    payload->sleepTime = secsBetweenThreads;
    pthread_t packager;
    pthread_create(&packager, NULL, packageSubtasks, (void*) payload);
    pthread_join(packager, NULL);
    printf("Messages sent?\n");

    // Cleanup
    freeMatrix(first);
    freeMatrix(second);
    free(payload);
    return 0;
}
