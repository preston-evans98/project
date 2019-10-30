#ifndef __UTILS__H__
#define __UTILS__H__
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct QueueMessage {
    long type;
    int jobid;
    int rowvec;
    int colvec;
    int innerDim;
    int data[100];
} Msg;

void* safeMalloc(int size);
int max(int a, int b);

#endif
