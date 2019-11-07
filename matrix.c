#include "matrix.h"
#define BUFF_SIZE 1000

int **allocateMatrix(int rows, int cols)
{
    int **new = safeMalloc(sizeof(int *) * rows);
    for (int i = 0; i < rows; i++)
    {
        new[i] = safeMalloc(sizeof(int) * cols);
    }
    return new;
}

Matrix *newMatrix(int rows, int cols)
{
    Matrix *new = safeMalloc(sizeof(Matrix));
    new->rows = rows;
    new->cols = cols;
    new->values = allocateMatrix(rows, cols);
    return new;
}

Matrix *readMatrixFromFile(FILE *file)
{
    Matrix *new = safeMalloc(sizeof(Matrix));
    new->rows = 0;
    new->cols = 0;
    fscanf(file, "%d", &new->rows);
    fscanf(file, "%d", &new->cols);
    if (new->rows == 0 || new->cols == 0)
    {
        fprintf(stderr, "Unable to read matrix file. Please double check your formatting.\n");
        exit(0);
    }
    new->values = allocateMatrix(new->rows, new->cols);
    for (int i = 0; i < new->rows; i++)
    {
        for (int j = 0; j < new->cols; j++)
        {
            fscanf(file, "%d", &(new->values[i][j]));
            if (feof(file))
            {
                int failedIndex = (i * new->cols) + j + 1;
                fprintf(stderr, "Failed to read matrix file at %dth value. Please double check your formatting.\n", failedIndex);
                exit(0);
            }
        }
    }
    return new;
}

void freeMatrix(Matrix *m)
{
    for (int i = 0; i < m->rows; i++)
    {
        free(m->values[i]);
    }
    free(m->values);
    free(m);
}

void printMatrix(Matrix *m)
{
    if (!m)
        return;
    for (int i = 0; i < m->rows; i++)
    {
        printf("[%d", m->values[i][0]);
        for (int j = 1; j < m->cols; j++)
        {
            printf(" %d", m->values[i][j]);
        }
        printf("]\n");
    }
}
