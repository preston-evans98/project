#ifndef __MATRIX__H__
#define __MATRIX__H__
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

typedef struct _matrix_ {
    int rows;
    int cols;
    int** values;
} Matrix;

Matrix* readMatrixFromFile(FILE* fp);
void printMatrix(Matrix* m);

#endif
