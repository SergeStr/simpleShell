/**************************************************************
* Class::  CSC-415-0# Summer 2024
* Name::
* Student ID::
* GitHub-Name::
* Project:: Assignment 3 – Simple Shell with Pipes
*
* File:: <name of this file>
*
* Description::
*
**************************************************************/
#include <stdio.h>

int main(int argc, char* argv[]) {
    char* buffer = malloc(sizeof(char)*159);
    char* prompt = ">";
    if (argc > 1) {
        prompt = argv[1];
    }

    int working = 1;
    while (working == 1) {
        printf("%s ", prompt);

    }

    free(buffer);
    return 0;
}