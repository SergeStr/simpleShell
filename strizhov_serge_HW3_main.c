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
#include <sys/wait.h>

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
    while (working) {
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
            // Sweeping the buffer without modifying it.
            char* bufferLoc = strchr(buffer, ' ');
            // To account for null term and no space at the end,
            // This starts at two.
            int countOfArgs = 2;
            while (bufferLoc != NULL){
                countOfArgs++;
                bufferLoc = bufferLoc + 1;
                bufferLoc = strchr(bufferLoc, ' ');
            }
            // Sweeping the buffer again and this time
            // Modifying via tokenization.
            char* args[countOfArgs];
            char* execName;
            char* arg = strtok(buffer, " ");
            execName = arg;
            for(int i = 0; i < countOfArgs-1; i++) {
                args[i] = arg;
                arg = strtok(NULL, " ");
            }
            args[countOfArgs-1] = NULL;

            pid_t exe = fork();
            if (exe < 0) {
                printf("Error making fork! Terminating.\n");
                working = 0;
                continue;
            }
            else if (exe == 0){
                execvp(execName, args);
                return 1; //Failsafe in case execvp doesn't do anything.
            }
            else {
                int result;
                waitpid(exe, &result, 0);
                if (WIFEXITED(result)) {
                    printf("Child %d, exited with %d\n", exe, WEXITSTATUS(result));
                }
                else {
                    printf("Child terminated irregularly. Terminating program.\n");
                    working = 0;
                    continue;
                }
            }
        }
    }

    free(buffer);
    return 0;
}