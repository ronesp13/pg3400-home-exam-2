#include <ctype.h>
#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "secretCoder.h"

#ifndef UTIL
#define UTIL "util.h"
#include UTIL
#endif

#ifndef QUEUE
#define QUEUE "queue.h"
#include QUEUE
#endif

#define DICTIONARY_PATH "/usr/share/dict/words"
#define SONG_LIBRARY_PATH "songLibrary/"

char *preprocessKey(const char *inputMessageFile, int *status) {
    FILE *file = fopen(inputMessageFile, "r");
    if (file == NULL) {
        *status = KEY_FILE_NOT_FOUND;
        return NULL;
    }

    char *key = (char*) allocate(sizeof(char));
    int reallocValue = 1;
    int count = 0;
    char input;

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

char** getDictionary(int *size) {
    FILE* file = fopen(DICTIONARY_PATH, "r");
    int wordCount = 0;
    char **array = (char**) allocate(sizeof(char*));
    char buffer[25];
    int reallocValue = 1;

    while (fgets(buffer, 25, file) != NULL) {
        buffer[strlen(buffer) - 1] = '\0';
        if (wordCount == reallocValue) {
            reallocValue *= 2;
            array = (char**) reallocate(array, sizeof(char*) * reallocValue);
        }
        array[wordCount] = (char*) allocate(sizeof(char) * strlen(buffer) + 1);
        array[wordCount] = strncpy(array[wordCount], buffer, strlen(buffer) + 1);
        wordCount++;
    }
    array = (char**) reallocate(array, sizeof(char*) * wordCount);
    *size = wordCount;
    fclose(file);
    return array;
}

Queue* getKeys() {
    Queue *queue = (Queue*) allocate(sizeof(Queue));
    queue->first = NULL;
    queue->last = NULL;
    DIR *directory = opendir(SONG_LIBRARY_PATH);
    struct dirent *entry = NULL;
    char filename[42] = {};

    if (directory) {
        while ((entry = readdir(directory)) != NULL) {
            if (entry->d_name[0] == '.') continue;
            sprintf(filename, "%s%s", SONG_LIBRARY_PATH, entry->d_name);
            char* key = preprocessKey(filename, 0);
            enqueue(queue, key);
        }
    }
    closedir(directory);
    return queue;
}

bool binarySearch(char* search, char** dictionary, int dictionarySize) {
    int first = 0;
    int last = dictionarySize - 1;
    int middle = (first + last) / 2;

    while (first <= last) {
        if (strcasecmp(dictionary[middle], search) < 0) {
            first = middle + 1;
        } else if (strcasecmp(dictionary[middle], search) == 0) {
            return true;
        } else {
            last = middle - 1;
        }
        middle = (first + last) / 2;
    }
    if (first > last) {
        return false;
    }
    return false;
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
    if (file == NULL) {
        *status = INPUT_FILE_NOT_FOUND;
        return NULL;
    }

    char input;
    int keySize = (int) strlen(key);
    int lowerLimit = 0;
    int upperLimit = 0;
    char *encodedMessage = calloc(1, sizeof(char));
    int encodedMessageSize = 0;

    while ((input = fgetc(file)) != EOF) {
        if (isalpha(input)) {
            char lowerCasedInput = tolower(input);
            bool found = false;

            // search the lower bounds of the array
            for (int i = lowerLimit; i >= 0; i--) {
                found = isLetterInKey(input, lowerCasedInput, key[i], i, distance, &lowerLimit, &upperLimit, &encodedMessage, &encodedMessageSize);
                if (found) {
                    break;
                }
            }

            if (found) continue;

            // search the upper bounds of the array
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
    if (key == NULL) {
        return NULL;
    }
    char *encoded = getEncodedMessage(inputMessageFile, key, status, distance);
    free(key);
    return encoded;
}

char *getDecodedMessage(const char *inputCodeFile, const char *key, int *status) {
    FILE *file = fopen(inputCodeFile, "r");
    if (file == NULL) {
        *status = INPUT_FILE_NOT_FOUND;
        return NULL;
    }
    char *decodedMessage = allocate(sizeof(char));
    char input;
    int position;
    int reallocValue = 1;
    int count = 0;

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
    if (key == NULL) {
        return NULL;
    }
    char *decodedMessage = getDecodedMessage(inputCodeFile, key, status);
    free(key);
    return decodedMessage;
}

char **hack(const char *inputCodeFile, int *status, int *size) {
    FILE *file = fopen(inputCodeFile, "r");
    if (file == NULL) {
        *status = FILE_NOT_FOUND;
        return NULL;
    }
    int dictionarySize = 0;
    char **dictionary = getDictionary(&dictionarySize);

    Queue *keys = getKeys();
    int position = 0;
    char input = '\0';
    int *positions = NULL;
    char *word = NULL;
    int wordLength = 0;
    int reallocValue = 1;

    while (true) {

        while (fscanf(file, "[%d]", &position) == 1) {
            if (wordLength == 0) {
                positions = (int*) allocate(sizeof(int));
            } else {
                if (wordLength == reallocValue) {
                    reallocValue *= 2;
                    positions = (int*) reallocate(positions, sizeof(int) * reallocValue);
                }
            }
            positions[wordLength] = position;
            wordLength++;
        }

        if (wordLength != 0) {
            positions = (int*) reallocate(positions, sizeof(int) * wordLength);
            Queue possibleKeys = { NULL, NULL};
            while (keys->first != NULL) {
                char *key = dequeue(keys);
                word = (char*) allocate(sizeof(char) * (wordLength + 1));
                for (int i = 0; i < wordLength; i++) {
                    int index = positions[i] < 0 ? positions[i] * -1 : positions[i];
                    word[i] = positions[i] < 0 ? toupper(key[index - 1]) : key[index - 1];
                }
                word[wordLength] = '\0';
                bool result = binarySearch(word, dictionary, dictionarySize);
                if (result) {
                    enqueue(&possibleKeys, key);
                } else {
                    free(key);
                }
                free(word);
                word = NULL;
            }
            *keys = possibleKeys;
            free(positions);
            positions = NULL;
            wordLength = 0;
            reallocValue = 1;
        }
        if (fscanf(file, "%c", &input) == EOF) {
            break;
        }
    }
    fclose(file);

    char **messages = malloc(sizeof(char*));
    reallocValue = 1;
    int keysSize = 0;

    while (keys->first != NULL) {
        char *key = dequeue(keys);
        char *encodedMessage = getDecodedMessage(inputCodeFile, key, status);
        if (reallocValue == keysSize) {
            reallocValue *= 2;
            messages = reallocate(messages, sizeof(char*) * reallocValue);
        }
        messages[keysSize] = encodedMessage;
        keysSize++;
        free(key);
    }
    *size = keysSize;

    for (int i = 0; i < dictionarySize; i++) {
        free(dictionary[i]);
    }
    free(dictionary);
    free(keys);
    return messages;
}