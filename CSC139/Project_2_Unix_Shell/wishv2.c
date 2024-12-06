#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_INPUT_SIZE 1024
#define MAX_ARG_SIZE 64
#define MAX_PATH_SIZE 64

char* path[MAX_PATH_SIZE];
const char error_message[30] = "An error has occurred\n";

void handle_cd(char* args[]) {
    if (args[1] == NULL || args[2] != NULL) {
        write(STDERR_FILENO, error_message, strlen(error_message));
    } else {
        if (chdir(args[1]) != 0) {
            write(STDERR_FILENO, error_message, strlen(error_message));
        }
    }
}

void handle_path(char* args[]) {
    int i = 0;
    while (args[i] != NULL) {
        path[i] = args[i];
        i++;
    }
    path[i] = NULL;
}

void execute_command(char* args[]) {
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        // Child process
        char* full_path = NULL;
        int j = 0;
        while (path[j] != NULL) {
            full_path = (char*)malloc(strlen(path[j]) + strlen("/") + strlen(args[0]) + 1);
            if (full_path == NULL) {
                perror("malloc");
                exit(1);
            }
            strcpy(full_path, path[j]);
            strcat(full_path, "/");
            strcat(full_path, args[0]);
            if (access(full_path, X_OK) == 0) {
                execv(full_path, args);
                free(full_path); // Free the memory allocated for full_path
                break; // If execution is successful, exit the loop
            }
            free(full_path); // Free the memory allocated for full_path
            j++;
        }

        // If we reached here, the command was not found in any path
        write(STDERR_FILENO, error_message, strlen(error_message));
        exit(1);
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
    }
}

int main(int argc, char* argv[]) {
    if (argc > 2) {
        write(STDERR_FILENO, error_message, strlen(error_message));
        exit(1);
    }

    // Initialize path with a NULL directory
    path[0] = NULL;

    if (argc == 2) {
        // Batch mode: Read commands from the specified file
        FILE* batch_file = fopen(argv[1], "r");
        if (batch_file == NULL) {
            perror("fopen");
            exit(1);
        }

        char* input = NULL;
        size_t input_size = 0;
        ssize_t line_size;

        while ((line_size = getline(&input, &input_size, batch_file)) != -1) {
            input[line_size - 1] = '\0'; // Remove the newline character

            char* args[MAX_ARG_SIZE];
            char* token = strtok(input, " \t");
            int arg_count = 0;

            while (token != NULL) {
                args[arg_count] = token;
                arg_count++;
                token = strtok(NULL, " \t");
            }
            args[arg_count] = NULL;

            if (arg_count > 0) {
                if (strcmp(args[0], "exit") == 0) {
                    if (arg_count > 1) {
                        // Print an error message to stderr
                        write(STDERR_FILENO, error_message, strlen(error_message));
                    } else {
                        // Close the batch file and exit the shell
                        fclose(batch_file);
                        exit(0);
                    }
                } else if (strcmp(args[0], "cd") == 0) {
                    // Handle the 'cd' command
                    handle_cd(args);
                } else if (strcmp(args[0], "path") == 0) {
                    // Handle the 'path' command
                    handle_path(args);
                } else {
                    // Execute the specified command
                    execute_command(args);
                }
            }
        }
        fclose(batch_file);
    } else {
        // Interactive mode: Prompt the user for input
        char* input = NULL;
        size_t input_size = 0;

        while (1) {
            printf("wish> ");
            size_t line_size;
            if ((line_size = getline(&input, &input_size, stdin)) == -1) {
                perror("getline");
                exit(1);
            }
            input[line_size - 1] = '\0'; // Remove the newline character

            char* args[MAX_ARG_SIZE];
            char* token = strtok(input, " \t");
            int arg_count = 0;

            while (token != NULL) {
                args[arg_count] = token;
                arg_count++;
                token = strtok(NULL, " \t");
            }
            args[arg_count] = NULL;

            if (arg_count > 0) {
                if (strcmp(args[0], "exit") == 0) {
                    if (arg_count > 1) {
                        char error_message[30] = "An error has occurred\n";
                        write(STDERR_FILENO, error_message, strlen(error_message));
                    } else {
                        free(input);
                        exit(0);
                    }
                } else if (strcmp(args[0], "cd") == 0) {
                    handle_cd(args);
                } else if (strcmp(args[0], "path") == 0) {
                    // Handle the 'path' command
                    handle_path(args);
                } else {
                    // Execute the specified command
                    execute_command(args);
                }
            }
        }
        free(input);
    }
    return 0;
}
