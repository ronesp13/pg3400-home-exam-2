#include <ctype.h>
#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef UTIL
#define UTIL "util.h"
#include UTIL
#endif
#include "secretCoder.h"

#define SUCCESS 200
#define FILE_NOT_FOUND 404

char *preprocessKey(const char *inputMessageFile, int *status) {
    FILE *file = NULL;
    char *key = (char*) allocate(sizeof(char));
    int reallocValue = 1;
    int count = 0;
    char input;

    file = fopen(inputMessageFile, "r");
    if (file == NULL) {
        *status = FILE_NOT_FOUND;
        return NULL;
    }

    while ((input = (char) fgetc(file)) != EOF) {
        if (reallocValue == count) {
            reallocValue *= 2;
            key = (char*) reallocate(key, sizeof(char) * reallocValue);
        }
        if (isalpha(input)) {
            key[count] = isupper(input) ? (char) tolower(input) : input;
            count++;
        }
    }
    key = reallocate(key, sizeof(char) * (count + 1));
    key[count] = '\0';
    fclose(file);
    return key;
}

char *encode(const char *inputMessageFile, const char *keyFile, int *status, int distance) {
    char *key = preprocessKey(keyFile, status);

    free(key);
    return NULL;
}

char *decode(const char *inputCodeFile, const char *keyFile, int *status) {
    return NULL;
}

char *hack(const char *inputCodeFile, int *status) {
    return NULL;
}
