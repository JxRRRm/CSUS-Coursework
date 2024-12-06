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
          if (execvp(args[0], args) == -1) {
            // Check if it's a built-in command, and handle it
            if (strcmp(args[0], "cd") == 0) {
                handle_cd(args);
            } else if (strcmp(args[0], "path") == 0) {
                handle_path(args);
            } else {
                write(STDERR_FILENO, error_message, strlen(error_message));
                exit(1);
            }
        } else {
            // Parent process
            int status;
            waitpid(pid, &status, 0);
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc > 2) {
        write(STDERR_FILENO, error_message, strlen(error_message));
        exit(1);
    }

    path[0] = NULL; // Initialize path

    if (argc == 2) {
        // Batch mode: Read commands from the specified file
        FILE* batch_file = fopen(argv[1], "r");
        if (batch_file == NULL) {
            perror("fopen");
            exit(1);
        }

        char input[MAX_INPUT_SIZE];

        while (fgets(input, sizeof(input), batch_file) != NULL) {
            input[strcspn(input, "\n")] = '\0';

            char *args[MAX_ARG_SIZE];
            char *token = strtok(input, " \t");
            int arg_count = 0;
            int redirect_output = 0;

            while (token != NULL) {
                if (strcmp(token, ">") == 0) {
                    redirect_output = 1;
                } else {
                    args[arg_count] = token;
                    arg_count++;
                }
                token = strtok(NULL, " \t");
            }
            args[arg_count] = NULL;

            if (arg_count > 0) {
                if (strcmp(args[0], "exit") == 0) {
                    if (arg_count > 1) {
                        char error_message[30] = "An error has occurred\n";
                        write(STDERR_FILENO, error_message, strlen(error_message));
                    } else {
                        fclose(batch_file); // Close the batch file
                        exit(0);
                    }
                } else if (strcmp(args[0], "cd") == 0) {
                    handle_cd(args);
                } else if (strcmp(args[0], "path") == 0) {
                    handle_path(args);
                } else {
                    if (redirect_output) {
                        int fd = open(args[arg_count - 1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
                        if (fd < 0) {
                            perror("open");
                            exit(1);
                        }
                        dup2(fd, STDOUT_FILENO);
                        close(fd);
                        args[arg_count - 1] = NULL;
                    }
                    execute_command(args);
                }
            }
        }
        fclose(batch_file);
    } else {
        // Interactive mode: Prompt the user for input
        char *input = NULL;
        size_t input_size = 0;

        while (1) {
            printf("wish> ");
            if (getline(&input, &input_size, stdin) == -1) {
                perror("getline");
                exit(1);
            }
            input[strcspn(input, "\n")] = '\0';

            char *args[MAX_ARG_SIZE];
            char *token = strtok(input, " \t");
            int arg_count = 0;
            int redirect_output = 0;

            while (token != NULL) {
                if (strcmp(token, ">") == 0) {
                    redirect_output = 1;
                } else {
                    args[arg_count] = token;
                    arg_count++;
                }
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
                    handle_path(args);
                } else {
                    if (redirect_output) {
                        int fd = open(args[arg_count - 1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
                        if (fd < 0) {
                            perror("open");
                            free(input);
                            exit(1);
                        }
                        dup2(fd, STDOUT_FILENO);
                        close(fd);
                        args[arg_count - 1] = NULL;
                    }
                    execute_command(args);
                }
            }
        }
        free(input);
    }
    return 0;
}
