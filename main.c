#include <stdio.h>
#include <stdlib.h>

#include "secretCoder.h"

int main(int argc, char **argv) {
    int status = 0;
    encode("inputMessageFiles/hello.txt", "songLibrary/sweetChildGR.txt", &status, 50);
    decode("inputCodeFiles/hello_enc.txt", "songLibrary/sweetChildGR.txt", &status);
}