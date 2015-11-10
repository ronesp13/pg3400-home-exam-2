#include <stdio.h>
#include <stdlib.h>

#include "secretCoder.h"

int main(int argc, char **argv) {
    encode("inputMessageFiles/hello.txt", "songLibrary/sweetChildGR.txt", NULL, 0);
}