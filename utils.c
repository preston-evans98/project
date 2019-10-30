#include "utils.h"

void* safeMalloc(int size) {
    void* address = malloc(size);
    if (address == NULL) {
        fprintf(stderr, "Error: out of memory ");
        exit(-1);
    }
    return address;
}

int max(int a, int b) {
    if (a > b) {
        return a;
    }
    return b;
}
