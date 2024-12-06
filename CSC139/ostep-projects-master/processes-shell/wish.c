#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <ctype.h>
// Define constants
#define MAX_INPUT_SIZE 1024
#define MAX_ARG_SIZE 64
#define MAX_PATH_SIZE 64
const char error_message[30] = "An error has occurred\n";
char* path[MAX_PATH_SIZE];
// Flags
int output_redirection = 0;
int redirection_error = 0;
int parallel_mode = 0;
// Function declaration
void exe_batch_mode(FILE* batch_file);                          // 1. Executes batch mode
void exe_interactive_mode();                                    // 2. Executes interactive mode
void exe_cmd(char* args[]);                                     // 3. Executes external commands
void parse_input(char* input, char* args[], int* arg_count);    // 4. Parses input into arguments
void exe_path(char* args[]);                                    // 5. Executes the built-in command, 'path'
void exe_cd(char* arg[]);                                       // 6. Executes the built-in command, 'cd'
void check_for_redir(char* args[]);                             // 7. Checks for output redirection
void check_for_redir_errors(char* filename);                    // 8. Checks input for syntax errors regarding redirection
void redirect_output(char* filename);                           // 9. Redirects output to the specified file
void check_parallel(char* input);                               // 10. Checks input for parallel commands
void parallel_parse(char* input, char* args[], int* arg_count); // 11. Separates parallel commands into different strings
void trimSpaces(char* str);                                     // 12. Function to trim leading and trailing spaces from a string
//=================================================================================================================
// MAIN
int main(int argc, char* argv[]) {
    if (argc > 2) {
        write(STDERR_FILENO, error_message, strlen(error_message));
        exit(1);    
    }
    path[0] = "/bin"; // Initialize path with the /bin directory
    if (argc == 2) {    
        // Batch mode
        // Open batch file
        FILE* batch_file = fopen(argv[1], "r");
        if (batch_file == NULL) {
            write(STDERR_FILENO, error_message, strlen(error_message));
            exit(1);
        }
        exe_batch_mode(batch_file);
        fclose(batch_file);
    } else {    
        // Interactive mode
        exe_interactive_mode();
    }
    return 0;
}// End main
//=================================================================================================================
// 1.
void exe_batch_mode(FILE* batch_file) {
    char* input = NULL;     // Used to store user input
    size_t input_size = 0;  // Size of user input
    while (getline(&input, &input_size, batch_file) != -1) {
        input[strcspn(input, "\n")] = '\0'; // Replace newline char with null char
        check_parallel(input);  // Check for parallel commands
        if (parallel_mode == 1) {
            // Parallel commands detected
            char* parallel_cmds[MAX_ARG_SIZE];
            int cmd_count;
            parallel_parse (input, parallel_cmds, &cmd_count);  // separate commands
           int i = 0;
            while (cmd_count != 0) {
                char* args[MAX_ARG_SIZE];
                int arg_count;
                parse_input(parallel_cmds[i], args, &arg_count);  // parse input
                if (arg_count > 0) {
                    // Check if it's a built-in command and handle it
                    if (strcmp(args[0], "exit") == 0) {
                        if (arg_count > 1) {
                            write(STDERR_FILENO, error_message, strlen(error_message));
                        } else {
                            exit(0);
                        }
                    } else if (strcmp(args[0], "cd") == 0) {
                        if (arg_count != 2) {    // cd always take one argument
                            write(STDERR_FILENO, error_message, strlen(error_message));
                        } else {
                            exe_cd(args);
                        }
                    // path command
                    } else if (strcmp(args[0], "path") == 0) {
                        exe_path(args);
                    } else {    // If we reach here, the command is not a built-in command
                        exe_cmd(args);  
                    }
                }
                cmd_count--;
                i++;
            }
        } else {
            // Parallel mode off
            char* args[MAX_ARG_SIZE];
            int arg_count;
            //printf("Parallel commands NOT detected...\n");
            parse_input(input, args, &arg_count);  // parse input
            if (arg_count > 0) {
                // Check if it's a built-in command and handle it
                if (strcmp(args[0], "exit") == 0) {
                    if (arg_count > 1) {
                        write(STDERR_FILENO, error_message, strlen(error_message));
                    } else {
                        exit(0);
                    }
                } else if (strcmp(args[0], "cd") == 0) {
                    if (arg_count != 2) {    // cd always take one argument
                        write(STDERR_FILENO, error_message, strlen(error_message));
                    } else {
                        exe_cd(args);
                    }
                // path command
                } else if (strcmp(args[0], "path") == 0) {
                    exe_path(args);
                } else {    // If we reach here, the command is not a built-in command
                    exe_cmd(args);  
                }
            }
        }   
        }// End while loop
}// End exe_batch_mode
//=================================================================================================================
// 2.
void exe_interactive_mode() {
    char* input = NULL;     // Used to store user input
    size_t input_size = 0;  // Size of user input
    // Infinite Loop
    while (1) {
        printf("wish> ");   // Print prompt
        // Get user input
        if (getline(&input, &input_size, stdin) == -1) {
            if (feof(stdin)) {
                // EOF reached, exit gracefully
                free(input);  // Free allocated memory
                exit(0);      // Exit the program without errors
            } else {
                perror("getline");
                exit(1);
            }
        }// End get user input
        input[strcspn(input, "\n")] = '\0'; // Replace newline character with null character
        char* args[MAX_ARG_SIZE];   // Used to store arguments
        int arg_count;              // Use to track argument count 
        check_parallel(input);
        if (parallel_mode == 1){
            char* parallel_cmds[MAX_ARG_SIZE];
            int cmd_count;
            parallel_parse (input, parallel_cmds, &cmd_count);
            int i = 0;
            while (cmd_count != 0) {
                char* args[MAX_ARG_SIZE];
                int arg_count;
                parse_input(parallel_cmds[i], args, &arg_count);  // parse input
                // Try to execute commands
                if (arg_count > 0) {    
                    // Check if it's a built-in command and handle it
                    if (strcmp(args[0], "exit") == 0) {
                        if (arg_count > 1) {
                            write(STDERR_FILENO, error_message, strlen(error_message));
                        } else {
                            exit(0);
                        }
                    } else if (strcmp(args[0], "cd") == 0) {
                        if (arg_count != 2) {    // cd always take one argument
                            write(STDERR_FILENO, error_message, strlen(error_message));
                        } else {
                            exe_cd(args);
                        }
                    // path command
                    } else if (strcmp(args[0], "path") == 0) {
                        exe_path(args);
                    } else {    // If we reach here, the command is not a built-in command
                        exe_cmd(args);  
                    }
                }
                cmd_count--;
                i++;
            }
            /*
            */
        } else {
            parse_input(input, args, &arg_count);  // parse input
            // Try to execute commands
            if (arg_count > 0) {
                // Check if it's a built-in command and handle it
                if (strcmp(args[0], "exit") == 0) {
                    if (arg_count > 1) {
                        write(STDERR_FILENO, error_message, strlen(error_message));
                    } else {
                        exit(0);
                    }
                } else if (strcmp(args[0], "cd") == 0) {
                    if (arg_count != 2) {    // cd always take one argument
                        write(STDERR_FILENO, error_message, strlen(error_message));
                    } else {
                        exe_cd(args);
                    }
                // path command
                } else if (strcmp(args[0], "path") == 0) {
                    exe_path(args);
                } else {    // If we reach here, the command is not a built-in command
                    exe_cmd(args);  
                }
            }
        }
    }// End while loop
    free(input);
} // End exe_interactive_mode
//=================================================================================================================
// 3.
void exe_cmd(char* args[]) {
    pid_t process_id = fork();  // PID returned by fork()
    if (process_id == -1) {     // error in forking; no child process created
        perror("fork");
        exit(1);
    } else if (process_id == 0) {   // sucessful fork
        // Child process
        if (strcmp(args[0], "&") == 0) { return; }
        // Check for output redirection
        check_for_redir(args);
            // Iterate through the path directories and try to execute the command
            int i = 0;
            while (path[i] != NULL) {
                char command_path[MAX_INPUT_SIZE];
                snprintf(command_path, sizeof(command_path), "%s/%s", path[i], args[0]);
                // Check if the command is executable in the current path
                if (access(command_path, X_OK) == 0) {
                    // Execute the command from this directory
                    execv(command_path, args);
                    // If execv fails, exit the child process
                    perror("execv");
                    exit(1);
                }
                i++;
            }
            // If none of the directories had the executable, it's an error
            write(STDERR_FILENO, error_message, strlen(error_message));
            exit(1);
    } else {
        // Parent process
        int status;
        waitpid(process_id, &status, 0);
        // Restore standard output and error
        dup2(STDOUT_FILENO, 1);
        dup2(STDERR_FILENO, 2);
    }
} // End exe_cmd
//=================================================================================================================
// 4.
void parse_input(char* input, char* args[], int* arg_count){
    *arg_count = 0;
    char* token;
    char* saved_input = input;
    char* redirection = NULL;
    char* redir_args[MAX_ARG_SIZE];
    // Check for output redirection and separate arguments appropriately
    if ((redirection = strstr(saved_input, ">")) != NULL) {
        *redirection = '\0';
        redir_args[0] = saved_input;
        redir_args[1] = ">";
        redir_args[2] = redirection + 1;
        trimSpaces(redir_args[2]);
        redir_args[3] = NULL;  // Ensure the last element of args is NULL.
        char *delimiters = " ";
        while ((token = strsep(&redir_args[0], delimiters)) != NULL) {
            if (*token != '\0') { args[(*arg_count)++] = token; }
        }// End while loop
        args[(*arg_count)++] = redir_args[1];
        args[(*arg_count)++] = redir_args[2];
        // Ensure the last element of args is NULL (for exec functions)
        args[*arg_count] = NULL;
        return;
    }
    // Without redirection
    char *delimiters = " ";
    // Use strsep to tokenize the input string
    while ((token = strsep(&input, delimiters)) != NULL) {
        if (*token != '\0') { args[(*arg_count)++] = token; }
    }// End while loop
    // Ensure the last element of args is NULL (for exec functions)
    args[*arg_count] = NULL;
}// End parse_input
//=================================================================================================================
// 5.
void exe_path(char* args[]){
    int i = 0;
    while (args[i+1] != NULL) {
        char* dir = args[i+1];
        path[i] = strdup(dir);
        i++;
    }
    path[i] = NULL;
}
//=================================================================================================================
// 6.
void exe_cd(char* args[]) {
    // Attempt to change the current directory
    if (chdir(args[1]) != 0) {
        // Signal an error for chdir failure
        write(STDERR_FILENO, error_message, strlen(error_message));
    }
}
//=================================================================================================================
// 7.
void check_for_redir(char* args[]) {
    // Check for output redirection
    int j = 0;
    char* output_filename = NULL;
    output_redirection = 0; 
    while (args[j] != NULL) {
        if (strcmp(args[j], ">") == 0) {
            // Output redirection detected
            // first argument is '>'
            if (j == 0) {
                write(STDERR_FILENO, error_message, strlen(error_message));
                exit(1);
            }
            check_for_redir_errors(args[j + 1]);    // Check for redirection errors
            if (redirection_error == 1) {
                write(STDERR_FILENO, error_message, strlen(error_message));
                exit(1);
            } else {
                // No redirecction errors detected
                output_redirection = 1; // Set flag
                output_filename = args[j + 1]; 
                args[j] = NULL; 
                args[j + 1] = NULL;
                break;
            }
        }
        j++;
    }
    if (output_redirection) { redirect_output(output_filename); }   // Redirect output
}
//=================================================================================================================
// 8.
void check_for_redir_errors(char* filename) {
    // Check for NULL filename
    if (filename == NULL) { redirection_error = 1; }
    // Initialize variables for counting arguments and checking redirection
    int file_count = 0;
    char* token;
    char* filename_copy = strdup(filename);  // Create a copy to avoid modifying the original string
    // Tokenize the input using strsep and count arguments
    while ((token = strsep(&filename_copy, " ")) != NULL) {
        if (*token != '\0') {
            file_count++;
            if (strcmp(token, ">") == 0) { redirection_error = 1; }
        }
    }
    free(filename_copy);
    // Check for multiple arguments
    if (file_count > 1) { redirection_error = 1; }
}
//=================================================================================================================
// 9.
void redirect_output(char* filename) {
    int output_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP);
    if (output_fd == -1) {
        write(STDERR_FILENO, error_message, strlen(error_message));
        exit(1);
    }
    if (dup2(output_fd, STDOUT_FILENO) == -1) {
        perror("dup2");
        exit(1);
    }
    if (dup2(output_fd, STDERR_FILENO) == -1) {
        perror("dup2");
        exit(1);
    }
    close(output_fd);
}
//=================================================================================================================
// 10.
void check_parallel (char* input) {
    // Check for parallel commands
    if (strpbrk(input, "&") != NULL) { parallel_mode = 1; } 
    else { parallel_mode = 0; }
}
//=================================================================================================================
// 11.
void parallel_parse(char* input, char* parallel_cmds[], int* cmd_count) {
    *cmd_count = 0;
    char* input_copy = input;
    char* parallel = input;
    // Iterate through the input string
    while (*input_copy != '\0') {
        if (*input_copy == '&') {
            // Replace the '&' with a null character to terminate the current argument
            *input_copy = '\0';
            // Skip any whitespace before the parallel command
            while (*parallel == ' ' || *parallel == '\t') {
                parallel++;
            }
            // Store the argument in parallel_cmds
            parallel_cmds[(*cmd_count)++] = parallel;
            // Move to the next character after '&'
            parallel = input_copy + 1;
            // Skip any whitespace after the '&'
            while (*parallel == ' ' || *parallel == '\t') {
                parallel++;
            }
        }
        input_copy++;
    }
    // Handle the last argument after the loop
    if (input_copy != parallel) {
        // Skip any whitespace before the last command
        while (*parallel == ' ' || *parallel == '\t') {
            parallel++;
        }
        parallel_cmds[(*cmd_count)++] = parallel;
    }
    // Ensure the last element of parallel_cmds is NULL
    if (*cmd_count < MAX_ARG_SIZE) {
        parallel_cmds[*cmd_count] = NULL;
    }
}
//==================================================================================================
// 12.
void trimSpaces(char* str) {
    int start = 0, end = strlen(str) - 1;
    // Trim leading spaces
    while (isspace(str[start])) { start++; }

    // Trim trailing spaces
    while (end > start && isspace(str[end])) { end--; }
    // Shift the string to remove leading spaces
    int i;
    for (i = 0; i <= end - start; i++) {
        str[i] = str[start + i];
    }
    str[i] = '\0';
}
