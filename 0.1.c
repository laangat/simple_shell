#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

int main()
{
    char input[BUFFER_SIZE];
    while (1) {
        printf("$ ");
        fflush(stdout); // Make sure the prompt is displayed
        if (fgets(input, BUFFER_SIZE, stdin) == NULL) {
            // End of file, exit the program
            printf("\n");
            exit(EXIT_SUCCESS);
        }
        input[strcspn(input, "\n")] = '\0'; // Remove the newline character
        pid_t pid = fork();
        if (pid == -1) {
            // Fork error
            perror("fork");
        }
        else if (pid == 0) {
            // Child process
            execl(input, input, (char*) NULL);
            // If execl returns, an error occurred
            printf("Command not found: %s\n", input);
            exit(EXIT_FAILURE);
        }
        else {
            // Parent process
            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status)) {
                // Child process exited normally
                int exit_status = WEXITSTATUS(status);
                if (exit_status != 0) {
                    printf("Command failed with exit status %d\n", exit_status);
                }
            }
            else {
                // Child process exited abnormally
                printf("Command failed\n");
            }
        }
    }
    return 0;
}
