#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "matrix.h"
#include "utils.h"
#include "string.h"
#include <errno.h>

pthread_mutex_t sendLock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t receiveLock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t numJobsLock = PTHREAD_MUTEX_INITIALIZER;
int jobsSent = 0;
int jobsReceived = 0;
// int numJobs = -1;

typedef struct _payload
{
    bool readOnly;
    int msqid;
} Payload;

void sigIntHandler(int signum)
{
    if (signum == SIGINT)
    {
        printf("Jobs sent: %d. Jobs received: %d\n", jobsSent, jobsReceived);
    }
}

int doDotProduct(void *args)
{
    Msg message;
    Payload *payload = (Payload *)args;
    // int localNumJobs = -1;
    int bytesReceived = -1;
    // int rc;
    while (1)
    {
        // bytesReceived = msgrcv(payload->msqid, &message, getMsgSize(100), 1, IPC_NOWAIT);
        bytesReceived = msgrcv(payload->msqid, &message, getMsgSize(100), 1, 0);

        if (errno == ENOMSG || bytesReceived == -1)
        // if (errno == ENOMSG)
        {
            continue;
        }
        else
        {
            printf("Receiving job id %d type 1 size %d\n", message.jobId, getMsgSize(message.innerDim * 2));
            bytesReceived = -1;
            pthread_mutex_lock(&receiveLock);
            ++jobsReceived;
            pthread_mutex_unlock(&receiveLock);
            int sum = 0;
            for (int i = 0; i < message.innerDim; i++)
            {
                sum += message.data[i] * message.data[i + message.innerDim];
            }
            if (payload->readOnly)
            {
                printf("Sum for cell %d,%d is %d\n", message.rowNum, message.colNum, sum);
            }
            else
            {
                message.type = 2;
                pthread_mutex_lock(&sendLock);
                ++jobsSent;
                pthread_mutex_unlock(&sendLock);
                message.data[0] = sum;
                int rc = msgsnd(payload->msqid, &message, getMsgSize(1), 0);
                printf("Sending job id %d type %d size %d (rc=%d)\n", message.jobId, 2, getMsgSize(1), rc);
            }
        }
    }
    return 0;
}

int main(int argc, char **argv)
{
    // Handle interrupts
    bool readOnly = false;
    signal(SIGINT, sigIntHandler);
    // Check for correct number of arguments
    if (argc < 2)
    {
        raiseError("Usage: ./compute <thread pool size> <-n for just read and output calculations>\n");
    }
    // Parse args
    int numThreads = atoi(argv[1]);
    if (numThreads == 0)
    {
        raiseError("Must initialize numthreads >= 1\n");
    }
    if (argc >= 3 && (strncmp(argv[2], "-n", 3) == 0))
    {
        readOnly = true;
    }

    int msqid = msgget(11793506, 0666 | IPC_CREAT);
    if (msqid == -1)
    {
        raiseError("Error: could not establish message queue.\n");
    }

    Payload *payload = safeMalloc(sizeof(Payload));
    payload->readOnly = readOnly;
    payload->msqid = msqid;

    pthread_t threadPool[numThreads];
    for (int i = 0; i < numThreads; i++)
    {
        pthread_create(&threadPool[i], NULL, (void *)doDotProduct, (void *)payload);
    }

    // free(payload);
    pthread_exit(0);
}
