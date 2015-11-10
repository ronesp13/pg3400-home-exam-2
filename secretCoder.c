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

bool isLetterInKey(const char *key, char input, char keyValue, int index, int distance, int *lowerLimit, int *upperLimit) {
    if (input == key[index]) {
        char buffer[10] = {};
        int position = index + 1;
        position = isupper(input) ? -position : position;
        *lowerLimit = index - distance;
        *upperLimit = index + distance;
        int size = sprintf(buffer, "[%d]", position);
        return true;
    }
    return false;
}

char *getEncodedMessage(const char *inputMessageFile, const char* key, int *status, int distance) {
    FILE *file = fopen(inputMessageFile, "r");
    char input;
    char buffer[10] = {};
    size_t keySize = strlen(key);
    int lowerLimit = 0;
    int upperLimit = 0;
    char *encodedMessage = calloc(1, sizeof(char));
    int encodedMessageSize = 0;

    while ((input = fgetc(file)) != EOF) {
        if (isalpha(input)) {
            char lowerCasedInput = tolower(input);
            bool found = false;
            for (int i = lowerLimit; i >= 0; i--) {
                found = isLetterInKey(key, lowerCasedInput, key[i], i, distance, &lowerLimit, &upperLimit);
                if (lowerCasedInput == key[i]) {
                    int position = i + 1;
//                    lowerLimit = i - distance;
//                    upperLimit = i + distance;
//                    found = true;
                    position = isupper(input) ? -position : position;
                    int size = sprintf(buffer, "[%d]", position);
                    encodedMessage = realloc(encodedMessage, sizeof(char) * (encodedMessageSize + size + 1));
                    encodedMessageSize += size;
                    encodedMessage = strncat(encodedMessage, buffer, size);
                    break;
                }
            }
            if (found) continue;

            for (int i = upperLimit; i < keySize; i++) {
                if (lowerCasedInput == key[i]) {
                    int position = i + 1;
                    lowerLimit = i - distance;
                    upperLimit = i + distance;
                    found = true;
                    position = isupper(input) ? -position : position;
                    int size = sprintf(buffer, "[%d]", position);
                    encodedMessage = realloc(encodedMessage, sizeof(char) * (encodedMessageSize + size + 1));
                    encodedMessageSize += size;
                    encodedMessage = strncat(encodedMessage, buffer, size);
                    break;
                }
            }
            if (!found) {
                printf("Unable to satisfy d=%d. Program terminating.", distance);
                exit(1);
            }
        } else {
            encodedMessage = realloc(encodedMessage, sizeof(char) * (encodedMessageSize + 2));
            encodedMessageSize++;
            encodedMessage[encodedMessageSize - 1] = input;
            encodedMessage[encodedMessageSize] = '\0';
        }
    }

    fclose(file);
    return encodedMessage;
}

char *encode(const char *inputMessageFile, const char *keyFile, int *status, int distance) {
    char *key = preprocessKey(keyFile, status);
    char *encoded = getEncodedMessage(inputMessageFile, key, status, distance);
    printf("%s", encoded);
    free(encoded);
    free(key);
    return NULL;
}

char *decode(const char *inputCodeFile, const char *keyFile, int *status) {
    return NULL;
}

char *hack(const char *inputCodeFile, int *status) {
    return NULL;
}
