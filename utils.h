#ifndef __UTILS__H__
#define __UTILS__H__
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct QueueMessage {
    long type;
    int jobId;
    int rowNum;
    int colNum;
    int innerDim;
    int numjobs;
    int data[100];
} Msg;


int getMsgSize(int bufSize);
void* safeMalloc(int size);
int max(int a, int b);
void raiseError(char* message);

#endif
