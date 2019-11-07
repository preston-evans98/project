#include "utils.h"

void* safeMalloc(int size) {
    void* address = malloc(size);
    if (address == NULL) {
        raiseError("Error: out of memory\n");
    }
    return address;
}

int max(int a, int b) {
    if (a > b) {
        return a;
    }
    return b;
}

void raiseError(char* message) {
    fprintf(stderr, "%s", message);
    exit(-1);
}

int getMsgSize(int bufSize) {
    return sizeof(Msg) - sizeof(int) * 100 + sizeof(int) * bufSize;
    // return (sizeof(Msg)) - ;
}
