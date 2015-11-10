#include <stdlib.h>

void *allocate(size_t size);
void *reallocate(void *pointer, size_t size);
void *openFile(const char *file);