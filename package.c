#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "matrix.h"
#include <errno.h>

typedef struct __payload_
{
    Matrix *first;
    Matrix *second;
    int rowNum;
    int colNum;
    int jobId;
    int msqid;
    Matrix *result;
} Payload;

pthread_mutex_t sendLock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t receiveLock = PTHREAD_MUTEX_INITIALIZER;
int jobsSent = 0;
int jobsReceived = 0;

void sigIntHandler(int signum)
{
    if (signum == SIGINT)
    {
        printf("Jobs sent: %d. Jobs received: %d\n", jobsSent, jobsReceived);
    }
}

void *packageSubtasks(void *payload)
{
    Payload *data = (Payload *)payload;
    Matrix *first = data->first;
    Matrix *second = data->second;

    if (first == NULL || second == NULL)
    {
        fprintf(stderr, "Error: Null matrix passed to packageSubtasks thread %d\n", data->jobId);
        fflush(stdout);
        exit(0);
    }

    int innerDim = first->cols;
    Msg message;

    // for (int i = 0; i < first->rows; i++) {
    //     for (int j = 0; j < second->cols; j++) {
    message.type = 1;
    message.jobId = data->jobId;
    message.rowNum = data->rowNum;
    message.colNum = data->colNum;
    message.innerDim = innerDim;
    message.numjobs = first->rows * second->cols;

    for (int currentIndex = 0; currentIndex < innerDim; currentIndex++)
    {
        message.data[currentIndex] = first->values[data->rowNum][currentIndex];
        message.data[currentIndex + innerDim] = second->values[currentIndex][data->colNum];
    }
    // Send message
    pthread_mutex_lock(&sendLock);
    ++jobsSent;
    pthread_mutex_unlock(&sendLock);
    int rc = -1;
    rc = msgsnd(data->msqid, &message, getMsgSize(2 * innerDim), 0);
    printf("Sending job id %d type %d size %d (rc=%d)\n", data->jobId, 1, getMsgSize(2 * innerDim), rc);

    // TODO: Add check for eventual message send

    pthread_mutex_lock(&receiveLock);
    ++jobsReceived;
    pthread_mutex_unlock(&receiveLock);
    // Wait for response
    msgrcv(data->msqid, &message, getMsgSize(100), 2, 0);
    printf("Receiving job id %d type %d size %d\n", message.jobId, 2, getMsgSize(1));

    ((Payload *)payload)->result->values[message.rowNum][message.colNum] = message.data[0];
    return NULL;
}

int main(int argc, char **argv)
{
    // Handle interrupts
    signal(SIGINT, sigIntHandler);
    // Check for correct number of arguments
    if (argc < 5)
    {
        fprintf(stderr, "Usage: ./package <matrix 1 file> <matrix 2 file> <output matrix data file> <secs between thread creation>\n");
        exit(0);
    }
    // Parse args and verify that all file paths can be opened
    FILE *inputOne;
    FILE *inputTwo;
    FILE *output;
    int secsBetweenThreads;
    inputOne = fopen(argv[1], "r");
    if (inputOne == NULL)
    {
        fprintf(stderr, "Could not open file '%s' - aborting.\n", argv[1]);
        exit(-1);
    }
    inputTwo = fopen(argv[2], "r");
    if (inputTwo == NULL)
    {
        fprintf(stderr, "Could not open file '%s' - aborting.\n", argv[2]);
        exit(-1);
    }
    output = fopen(argv[3], "w");
    if (output == NULL)
    {
        fprintf(stderr, "Could not open file '%s' - aborting.\n", argv[3]);
        exit(-1);
    }
    secsBetweenThreads = atoi(argv[4]);

    // Read data from files
    Matrix *first = readMatrixFromFile(inputOne);
    Matrix *second = readMatrixFromFile(inputTwo);
    if (first->cols != second->rows)
    {
        fprintf(stderr, "Error: Cannot multiply matrices with dimensions %dx%d, %dx%d", first->rows, first->cols, second->rows, second->cols);
        exit(-1);
    }

    // Spawn subtasks
    int numJobs = first->rows * second->cols;
    pthread_t packagers[numJobs];

    Payload **payloads = safeMalloc(sizeof(Payload *) * numJobs);
    Matrix *result = newMatrix(first->rows, second->cols);

    // establish msg queue
    key_t key = 11793506;
    int msqid = msgget(key, 0666 | IPC_CREAT);
    if (msqid == -1)
    {
        fprintf(stderr, "Error: could not establish message queue.\n");
        exit(-1);
    }
    int jobId = 0;
    for (int i = 0; i < first->rows; i++)
    {
        for (int j = 0; j < second->cols; j++)
        {
            // printf("Spawning %d size %d\n", jobId, getMsgSize(0));
            payloads[jobId] = safeMalloc(sizeof(Payload));
            payloads[jobId]->first = first;
            payloads[jobId]->second = second;
            payloads[jobId]->rowNum = i;
            payloads[jobId]->colNum = j;
            payloads[jobId]->jobId = jobId;
            payloads[jobId]->result = result;
            payloads[jobId]->msqid = msqid;
            pthread_create(&packagers[jobId], NULL, packageSubtasks, payloads[jobId]);
            ++jobId;
            sleep(secsBetweenThreads);
        }
    }

    void *dummy = 0;
    for (int i = 0; i < numJobs; i++)
    {
        pthread_join(packagers[i], dummy);
    }

    printMatrix(result);
    // Cleanup
    freeMatrix(first);
    // for (int i = 0; i < second->rows; i++)
    // {
    //     free(second->values[i]);
    // }
    // free(second->values);
    // free(second);
    // freeMatrix(second);
    // freeMatrix(result);
    for (int i = 0; i < numJobs; i++)
    {
        free(payloads[i]);
    }
    free(payloads);

    return 0;
}
