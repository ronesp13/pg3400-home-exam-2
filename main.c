#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "secretCoder.h"

void printHelp() {
    printf("+--------------- W E S T E R G A T E --------------+\n"
           "| enc: ./westergate -enc messageFile keyFile       |\n"
           "| dec: ./westergate -dec codedMessageFile keyFile  |\n"
           "| hax: ./westergate -hax codedMessageFile          |\n"
           "|                                                  |\n"
           "|        Read README.md for more information       |\n"
           "+--------------------------------------------------+\n");

}

void writeOutput(char *output, int status) {
    if (status == SUCCESS) {
//        writeEncodedFile(output);
        printf("%s", output);
    } else if (status == FILE_NOT_FOUND) {
        printf("An error occurred while reading a file.\n");
    } else if (status == INPUT_FILE_NOT_FOUND) {
        printf("Could not open the specified input file.\n");
    } else if (status == KEY_FILE_NOT_FOUND) {
        printf("Could not open the specified key file.\n");
    } else if (status == DISTANCE_NOT_SATISFIED) {
        printf("Could not satisfy the distance value.\n");
    }
}

int main(int argc, char **argv) {

    if (argc == 1) {
        printf("Invalid amount of arguments. Run ./westergate -h for help.\n");
        return 0;
    }

    int status = 0;
    char *output;
    char **hackedMessages;

    if (argc == 4 && strcmp(argv[1], "-enc") == 0) {
        output = encode(argv[2], argv[3], &status, 0);
    } else if (argc == 6 && strcmp(argv[1], "-enc") == 0 && strcmp(argv[4], "-d") == 0) {
        output = encode(argv[2], argv[3], &status, atoi(argv[5]));
    } else if (argc == 4 && strcmp(argv[1], "-dec") == 0) {
        output = decode(argv[2], argv[3], &status);
    } else if (argc == 3 && strcmp(argv[1], "-hax") == 0) {
        int size = 0;
        hackedMessages = hack(argv[2], &status, &size);
        if (size == 0) {
            printf("No keys produced a valid result.\n");
            return 0;
        }
        for (int i = 0; i < size; i++) {
            printf("Probable message #%d: %s\n\n", i + 1, hackedMessages[i]);
            free(hackedMessages[i]);
        }
        free(hackedMessages);
        return 0;
    } else if (argc == 2 && strcmp(argv[1], "-h") == 0) {
        printHelp();
        return 0;
    } else {
        printf("Invalid command. Run ./westergate -h for help.\n");
        return 0;
    }
    writeOutput(output, status);
    if (output != NULL) {
        free(output);
    }
    return 0;
}