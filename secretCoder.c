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
#define DISTANCE_NOT_SATISFIED 500

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

bool isLetterInKey(char input, char lowerCasedInput, char keyValue, int index, int distance, int *lowerLimit, int *upperLimit, char **encodedMessage, int *encodedMessageSize) {
    if (lowerCasedInput == keyValue) {
        char buffer[10] = {};
        int position = index + 1;
        position = isupper(input) ? position * -1 : position;
        *lowerLimit = index - distance;
        *upperLimit = index + distance;
        int size = sprintf(buffer, "[%d]", position);
        *encodedMessage = (char*) reallocate(*encodedMessage, sizeof(char) * (*encodedMessageSize + size + 1));
        *encodedMessageSize += size;
        *encodedMessage = strncat(*encodedMessage, buffer, size);
        return true;
    }
    return false;
}

char *getEncodedMessage(const char *inputMessageFile, const char* key, int *status, int distance) {
    FILE *file = fopen(inputMessageFile, "r");
    char input;
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
                found = isLetterInKey(input, lowerCasedInput, key[i], i, distance, &lowerLimit, &upperLimit, &encodedMessage, &encodedMessageSize);
                if (found) {
                    break;
                }
            }

            if (found) continue;

            for (int i = upperLimit; i < keySize; i++) {
                found = isLetterInKey(input, lowerCasedInput, key[i], i, distance, &lowerLimit, &upperLimit, &encodedMessage, &encodedMessageSize);
                if (found) {
                    break;
                }
            }
            if (!found) {
                *status = DISTANCE_NOT_SATISFIED;
                free(encodedMessage);
                return NULL;
            }
        } else {
            encodedMessage = reallocate(encodedMessage, sizeof(char) * (encodedMessageSize + 2));
            encodedMessageSize++;
            encodedMessage = strncat(encodedMessage, &input, sizeof(char));
        }
    }
    *status = SUCCESS;
    fclose(file);
    return encodedMessage;
}

char *encode(const char *inputMessageFile, const char *keyFile, int *status, int distance) {
    char *key = preprocessKey(keyFile, status);
    char *encoded = getEncodedMessage(inputMessageFile, key, status, distance);
    printf("%s\n", encoded);
    free(key);
    free(encoded);
    return NULL;
}

char *getDecodedMessage(const char *inputCodeFile, const char *key, int *status) {
    char *decodedMessage = allocate(sizeof(char));
    char input;
    int position;
    int reallocValue = 1;
    int count = 0;
    FILE *file = fopen(inputCodeFile, "r");
    if (file == NULL) {
        *status = FILE_NOT_FOUND;
        return NULL;
    }

    while (true) {
        if (count == reallocValue) {
            reallocValue *= 2;
            decodedMessage = reallocate(decodedMessage, sizeof(char) * reallocValue);
        }
        int readStatus = fscanf(file, "[%d]", &position);
        if (readStatus == 1) {
            int index = position < 0 ? position * -1 : position;
            char value = key[index - 1];
            input = position < 0 ? toupper(value) : value;
        } else if (readStatus == EOF) {
            break;
        } else {
            fscanf(file, "%c", &input);
        }
        decodedMessage[count] = input;
        count++;
    }
    decodedMessage = (char*) reallocate(decodedMessage, sizeof(char) * (count + 1));
    decodedMessage[count] = '\0';
    fclose(file);
    return decodedMessage;
}

char *decode(const char *inputCodeFile, const char *keyFile, int *status) {
    char *key = preprocessKey(keyFile, status);
    char *decodedMessage = getDecodedMessage(inputCodeFile, key, status);
    free(key);
    printf("%s\n", decodedMessage);
    free(decodedMessage);
    return NULL;
}

char *hack(const char *inputCodeFile, int *status) {
    return NULL;
}
