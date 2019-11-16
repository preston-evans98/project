#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

// This file assumes that compute is already running in another terminal
bool runTest(char *testInputOne, char *testInputTwo, char *resultFileName)
{
    char command[500];
    sprintf(command, "./package %s.dat %s.dat result.dat 0", testInputOne, testInputTwo);
    system(command);
    FILE *resultFp = fopen("result.dat", "r");
    if (!resultFp)
    {
        fprintf(stderr, "Could not open result file.");
    }
    FILE *expectedFp = fopen(resultFileName, "r");
    if (!expectedFp)
    {
        fprintf(stderr, "Could not open expected file.");
    }
    Matrix *actual = readMatrixFromFile(resultFp);
    Matrix *expected = readMatrixFromFile(expectedFp);
    if (!matrixEquals(actual, expected))
    {
        fprintf(stderr, "%s * %s != %s\n", testInputOne, testInputTwo, resultFileName);
        return false;
    }
    return true;
}

int main()
{
    bool success;
    success = runTest("matrix1", "matrix2", "expected3x5.dat");
    if (!success)
    {
        printf("FAILURE!\n");
        exit(-1);
    }
    success = runTest("large", "l3", "expected50x5.dat");
    if (!success)
    {
        printf("FAILURE!\n");
        exit(-1);
    }
    success = runTest("large", "large", "expected50x50.dat");
    if (!success)
    {
        printf("FAILURE!\n");
        exit(-1);
    }
    success = runTest("small", "small", "expected1x1.dat");
    if (!success)
    {
        printf("FAILURE!\n");
        exit(-1);
    }
    printf("All tests passed!\n");
    return 0;
}
