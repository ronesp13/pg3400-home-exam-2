#include <ctype.h>
#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "secretCoder.h"

char *preprocessKey(const char *inputMessageFile) {
    FILE *file = NULL;
    char *key = NULL;
    int reallocValue = 1;
    int count = 0;
    char input;
    if ((file = fopen(inputMessageFile, "r")) == NULL) {
        printf("Could not open file. Program terminating.\n");
        exit(-1);
    }
    if ((key = (char*) malloc(sizeof(char))) == NULL ) {
        printf("Could not allocate memory. Program terminating.\n");
        exit(-1);
    }
    while ((input = (char) fgetc(file)) != EOF) {
        if (reallocValue == count) {
            reallocValue *= 2;
            if ((key = (char*) realloc(key, sizeof(char) * reallocValue)) == NULL) {
                printf("Could not allocate memory. Program terminating.\n");
                exit(-1);
            }
        }
        if (isalpha(input)) {
            key[count] = isupper(input) ? (char) tolower(input) : input;
            count++;
        }
    }
    if ((key = (char*) realloc(key, sizeof(char) * (count + 1))) == NULL) {
        printf("Could not allocate memory. Program terminating.\n");
        exit(-1);
    }
    key[count] = '\0';
    fclose(file);
    return key;
}

char *encode(const char *inputMessageFile, const char *keyFile, int *status, int distance) {
    char *key = preprocessKey(keyFile);

    free(key);
    return NULL;
}

char *decode(const char *inputCodeFile, const char *keyFile, int *status) {
    return NULL;
}

char *hack(const char *inputCodeFile, int *status) {
    return NULL;
}
