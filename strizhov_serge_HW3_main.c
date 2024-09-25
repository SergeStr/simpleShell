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
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char* argv[]) {
    char* buffer = malloc(sizeof(char)*159);
    if(buffer == NULL) {
        printf("Buffer failed to malloc. Exiting.\n");
    }
    char* prompt = "> ";
    if (argc > 1) {
        prompt = argv[1];
    }

    int working = 1;
    while (working == 1) {
        printf("%s", prompt);

        char* result = fgets(buffer, sizeof(char)*159, stdin);
        if ((strlen(result) > 0) && (result[strlen(result)-1] == '\n')){
            result[strlen(result)-1]='\0';
        }
        // Error and exit checks as well as outputs.
        if(result == NULL){
            working = 0;
            continue;
        }
        else if(strlen(buffer) == 0) {
            printf("Error: No input. Please enter a command.\n");
            continue;
        }
        else if (strcmp(buffer, "exit") == 0) {
            printf("Thank you for using this Simple Shell. Exiting.\n");
            working = 0;
            continue;
        } 
        // Runs inputted command with the use of forking.
        // First checks for amount of args by using spaces
        // as delimeters, then populates a pointer array
        // full of substrings as a result of token
        // delimeters.
        else {
            int countOfArgs = 0;
            char* arg = strtok(buffer, " ");
            while (arg) {
                countOfArgs++;
                arg = strtok(NULL, " ");
            }

            // Need to delimeter count of args by 1 due to the initial argument being the command type. FIX IT!
            printf("Number of args: %d", countOfArgs);
            char* args[countOfArgs];
            char* exeName = strtok(buffer, " ");
            arg = strtok(exeName, " ");
            for(int i = 0; i < countOfArgs-1; i++) {
                args[i] = arg;
                printf("%s\n", arg);
                arg = strtok(NULL, " ");
            }
            args[countOfArgs] = "\0";

            pid_t exe = fork();
            if (exe < 0) {
                printf("Error making fork! Terminating.\n");
                working = 0;
                continue;
            }
            printf("Given command: %s\n", exeName);
            printf("PID of current process: %d\n", exe);
            break;
        }
    }

    free(buffer);
    return 0;
}