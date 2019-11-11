#ifndef __VECTOR_INT_C
#define __VECTOR_INT_C
#include "vector.h"
#include "utils.h"

Vector *newVector()
{
    Vector *new = safeMalloc(sizeof(Vector));
    new->size = 0;
    new->capacity = 1;
    new->contents = safeMalloc(sizeof(void *) * new->capacity);
    for (int i = 0; i < new->capacity; ++i)
    {
        new->contents[i] = NULL;
    }
    return new;
}

void pushToVec(Vector *vec, void *value)
{
    if (vec->size >= vec->capacity)
    {
        doubleVecSize(vec);
    }
    vec->contents[vec->size] = value;
    vec->size++;
}

void doubleVecSize(Vector *vec)
{
    vec->capacity *= 2;
    void **temp_contents = safeMalloc(sizeof(void *) * (vec)->capacity);
    memcpy(temp_contents, vec->contents, sizeof(void *) * vec->capacity / 2);
    free(vec->contents);
    vec->contents = temp_contents;
    for (int i = vec->size; i < vec->capacity; ++i)
    {
        vec->contents[i] = NULL;
    }
}

void *popFromVec(Vector *vec)
{
    if (vec->size == 0)
        return NULL;
    void *last = vec->contents[--vec->size];
    return last;
}

Vector *destroyVec(Vector *vec)
{
    free(vec->contents);
    free(vec);
    return NULL;
}

#endif
