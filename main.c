#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

}

int main(int argc, char **argv) {

    if (argc == 1) {
        printf("Invalid amount of arguments. Run ./westergate -h for help.\n");
        return 0;
    }

    int status = 0;
    char *output;
    if (argc == 4 && strcmp(argv[1], "-enc") == 0) {
        output = encode(argv[2], argv[3], &status, 0);
    } else if (argc == 6 && strcmp(argv[1], "-enc") == 0 && strcmp(argv[4], "-d") == 0) {
        output = encode(argv[2], argv[3], &status, atoi(argv[5]));
    } else if (argc == 4 && strcmp(argv[1], "-dec") == 0) {
        output = decode(argv[2], argv[3], &status);
    } else if (argc == 3 && strcmp(argv[1], "-hax") == 0) {
        output = hack(argv[2], &status);
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