/**************************************************************
* Class::  CSC-415-03 Fall 2024
* Name:: Serge Strizhov
* Student ID:: 922426905
* GitHub-Name:: SergeStr
* Project:: Assignment 3 – Simple Shell with Pipes
*
* File:: strizhov_serge_HW3_main.c
*
* Description:: Runs a unique simple shell inside of the terminal.
* Takes in user input, parses them as functions and their arguments
* and then executes. This simple shell supports piping, up until
* the command line limit of 159 bytes.
**************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    // 159 due to it being the limit of the command line
    // input.
    char* buffer = malloc(sizeof(char)*159);
    // Stores the original buffer location, due
    // to the buffer being constantly modified by
    // strtok_r(). Otherwise, results in a free()
    // error.
    char* originalBufferLocation = buffer;
    // Failsafe for whenever buffer doesn't malloc properly.
    if(buffer == NULL) {
        printf("Buffer failed to malloc. Exiting.\n");
    }
    // Sets up a placeholder prompt, which is overriden
    // by a given prompt if it exists.
    char* prompt = "> ";
    if (argc > 1) {
        prompt = argv[1];
    }

    // I like to terminate my user-oriented loops
    // By setting the working value to 0 and then
    // continuing, instead of breaking out.
    int working = 1;
    while (working) {
        printf("%s", prompt);
        // fgets is used as it can collect up to an X of 
        // byte data. scanf isn't very good at this.
        char* result = fgets(buffer, sizeof(char)*159, stdin);
        // Error and exit checks.
        if(result == NULL){ // Occurs on EOF
            working = 0;
            continue;
        }
        // Otherwise gets valid data, from which it removes 
        // newline character at the end that
        // fgets likes to take.
        else {
            if ((strlen(result) > 0) && (result[strlen(result)-1] == '\n')){
                result[strlen(result)-1]='\0';
            }
        }
        // Some predetermined data states and their results.
        if(strlen(buffer) == 0) {
            printf("Error: No input. Please enter a command.\n");
            continue;
        }
        else if (strcmp(buffer, "exit") == 0) {
            working = 0;
            continue;
        } 
        // Otherwise, most likely valid funcs. 
        // Sets up forking, piping, and passing
        // data between loops.
        pid_t exe;
        int pi[2];
        int data = 0;
        char* bufferLoc;
        // Splits up input by pipes to then do one by one.
        char* func = strtok_r(buffer, "|", &buffer);
        while (func != NULL) {
            // In the case of pipes, there
            // can be spaces before or after
            // the func. This removes the
            // 'before'.
            bufferLoc = strchr(func, ' ');
            if (bufferLoc == func) {
                bufferLoc++;
            }
            // Checks for the amount of spaces, so
            // it knows how big the argv list
            // should be.
            int countOfArgs = 2;
            while (bufferLoc != NULL){
                countOfArgs++;
                bufferLoc++;
                bufferLoc = strchr(bufferLoc, ' ');
            }
            // Removes the 'after' space
            if (func[strlen(func)-1] == ' ') {
                countOfArgs--;
            }
 
            // Setting up and population of argv
            char* args[countOfArgs];
            char* arg = strtok(func, " ");
            char* execName = arg;
            for(int i = 0; i < countOfArgs-1; i++) {
                args[i] = arg;
                arg = strtok(NULL, " ");
            }
            // Null termination for argv as its required
            args[countOfArgs-1] = NULL;

            // Sets up the next func for future loop.
            func = strtok_r(buffer, "|", &buffer);

            pipe(pi);
            exe = fork();

            // Terminates if the fork fucked up.
            if ((exe < 0)) {
                printf("Forking error. Terminating.\n");
                working = 0;
                continue;
            }
            // Executes the func with its func arg given.
            // IF there are future funcs (pipes) then
            // Stores STDOUT in 'data' variable
            // for next func to take it as STDIN
            // Also takes data outputted by prev
            // func and uses it as STDIN.
            else if (exe == 0) {
                close(pi[0]);
                dup2(data, STDIN_FILENO);
                if (func != NULL) {
                    dup2(pi[1], 1);
                }
                close(pi[1]);
                execvp(execName, args);
                // This should never happen. Hopefully.
                printf("Error executing instructions. Terminating.\n");
                return 1;
            }
            // Prints data of child process
            // such as its PID and execution result.
            // Closes its STDIN pipe if there are no
            // future funcs.
            else {
                int result;
                waitpid(exe, &result, 0);
                close(pi[1]);
                data = pi[0];
                if (WIFEXITED(result)) {
                    printf("Child %d, exited with %d\n", exe, WEXITSTATUS(result));
                }
                else {
                    printf("Child terminated irregularly. Terminating program.\n");
                    working = 0;
                    continue;
                }

                if (func == NULL){
                    close(pi[0]);
                }
            }
        }
    }

    free(originalBufferLocation);
    return 0;
}