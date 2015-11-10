#include <stdio.h>
#include <stdlib.h>
#include "util.h"

void *allocate(size_t size) {
    void *tmp = malloc(size);
    if (tmp == NULL) {
        printf("Could not allocate memory. Program terminating.\n");
        exit(-1);
    }
    return tmp;
}

void *reallocate(void *pointer, size_t size) {
    void *tmp = realloc(pointer, size);
    if (tmp == NULL) {
        printf("Could not reallocate memory. Program terminating.\n");
        exit(-1);
    }
    return tmp;
}