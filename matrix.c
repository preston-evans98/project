#include "matrix.h"
#include <stdio.h>
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

void fprintReadableMatrix(FILE *fp, Matrix *m)
{
    fprintf(fp, "%d %d\n", m->rows, m->cols);
    if (!m)
        return;
    for (int i = 0; i < m->rows; i++)
    {
        for (int j = 0; j < m->cols; j++)
        {
            fprintf(fp, "%d ", m->values[i][j]);
        }
    }
    fprintf(fp, "\n");
}
void fprintMatrix(FILE *fp, Matrix *m)
{
    if (!m)
        return;
    for (int i = 0; i < m->rows; i++)
    {
        for (int j = 0; j < m->cols; j++)
        {
            fprintf(fp, "%d ", m->values[i][j]);
        }
    }
    fprintf(fp, "\n");
}

bool matrixEquals(Matrix* m1, Matrix* m2) {
    if (!m1 || !m2) return false; 
    if (m1->rows != m2->rows) return false;
    if (m1->cols != m2->cols) return false;
    for (int i = 0; i < m1->rows; i++) {
        for (int j = 0; j < m1->cols; j++) {
            if (m1->values[i][j] != m2->values[i][j]) return false;
        }
    }
    return true;
}

