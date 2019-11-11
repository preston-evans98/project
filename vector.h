
#ifndef __VECTOR_INT_H
#define __VECTOR_INT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct _vector
{
    int size;
    int capacity;
    void **contents;
} Vector;

Vector *newVector();
void pushToVec(Vector *, void *);
void *popFromVec(Vector *);
Vector *destroyVec(Vector *);
void doubleVecSize(Vector *);

#endif
