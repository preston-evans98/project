#include "matrix.h"
#define BUFF_SIZE 1000

int** allocateMatrix(int rows, int cols) {
    int** new = safeMalloc((sizeof(int*) * rows));
    for(int i = 0; i < cols; i++) {
        new[i] = malloc(sizeof(int) * cols);
    }
    return new;
}

Matrix* readMatrixFromFile(FILE* file) {
    Matrix* new = safeMalloc(sizeof(Matrix));
    new->rows = 0;
    new->cols = 0;
    fscanf(file, "%d", &new->rows);
    fscanf(file, "%d", &new->cols);
    if (new->rows == 0 || new->cols == 0) {
        printf("Unable to read matrix file. Please double check your formating.\n");
        exit(0);
    }
    new->values = allocateMatrix(new->rows, new->cols);
    for(int i = 0; i < new->rows; i++) {
        for(int j = 0; j < new->cols; j++) {
            fscanf(file, "%d", &(new->values[i][j]));
            if (feof(file)) {
                int failedIndex = (i * new->cols) + j + 1;
                printf("Failed to read matrix file at %dth value. Please double check your formating.\n", failedIndex);
                exit(0);
            }
        }
    }
    return new;
}

void printMatrix(Matrix* m) {
    for(int i = 0; i < m->rows; i++) {
        printf("[%d", m->values[i][0]);
        for(int j = 1; j < m->cols; j++) {
            printf(" %d", m->values[i][j]);
        }
        printf("]\n");
    }
}
