#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <ctype.h> // Include for isspace function

// Define constants
#define MAX_INPUT_SIZE 1024
#define MAX_ARG_SIZE 64
#define MAX_PATH_SIZE 64

const char error_message[30] = "An error has occurred\n";
char* path[MAX_PATH_SIZE];
int redirection_error = 0;

// Function declaration
void parse_input(char* input, char* args[], int* arg_count);
void exe_batch_mode(FILE* batch_file);
void exe_interactive_mode();
void exe_cmd(char* args[]);
void exe_path(char* args[]);
void exe_cd(char* arg[]);
void redirect_output(char* filename);
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
            perror("fopen");
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
// BATCH
void exe_batch_mode(FILE* batch_file) {
    char input[MAX_INPUT_SIZE];

    while (fgets(input, sizeof(input), batch_file) != NULL) {
        input[strcspn(input, "\n")] = '\0'; // Replace newline char with null char

        char* args[MAX_ARG_SIZE];
        int arg_count;
        //char output_file[MAX_INPUT_SIZE]; // Create output_file here

        parse_input(input, args, &arg_count);  // parse input
        if (arg_count > 0) {
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
            } else {    // If we reach here, 
                exe_cmd(args);  
            }
        }
    }// End while loop
}// End exe_batch_mode
//=================================================================================================================
void exe_interactive_mode() {
    char* input = NULL;     // Used to store user input
    size_t input_size = 0;  // Size of user input
    //char output_file[MAX_INPUT_SIZE];   // Create output_file here

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
        // Parse input into arguments
        parse_input(input, args, &arg_count);  // parse input
         // Check if it's a built-in command and handle it
         if (arg_count > 0){
            // exit command
            if (strcmp(args[0], "exit") == 0) {
                if (arg_count > 1) {    // It is an error to pass any arguments to exit.
                    write(STDERR_FILENO, error_message, strlen(error_message));
                    exit(1);
                } else {
                    free(input);
                    exit(0);    // Exit Gracefully
                }
            // cd command
            } else if (strcmp(args[0], "cd") == 0) {
                if (arg_count != 2) {    // cd always take one argument
                    write(STDERR_FILENO, error_message, strlen(error_message));
                    exit(1);
                } else {
                    exe_cd(args);
                }
            // path command
            } else if (strcmp(args[0], "path") == 0) {
                exe_path(args);
            } else {    // If we reach here, 
                exe_cmd(args);  
            }
         }// End if (arg_count > 0)
    }// End while loop
    free(input);
} // End exe_interactive_mode
//=================================================================================================================
void exe_cmd(char* args[]) {
    pid_t process_id = fork();  // PID returned by fork()

    if (process_id == -1) {     // error in forking; no child process created
        perror("fork");
        exit(1);
    } else if (process_id == 0) {   // sucessful fork
        // Child process
        // Check for output redirection
        int j = 0;
        int output_redirection = 0;
        char* output_filename = NULL;

        while (args[j] != NULL) {
           if (strcmp(args[j], ">") == 0) {
                // Output redirection detected
                if (output_redirection || args[j + 1] == NULL || args[j + 2] != NULL) {
                    write(STDERR_FILENO, error_message, strlen(error_message));
                    exit(1);
                }
                output_redirection = 1;
                output_filename = args[j + 1];
                args[j] = NULL;
                break;
            }
            j++;
        }

        if (output_redirection) {
            redirect_output(output_filename);
        }
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
// PARSE function
void parse_input(char* input, char* args[], int* arg_count){
    *arg_count = 0;
    char* token;
    char* saved_input = input;
    char* redirection = NULL;

    // Check for output redirection
    if ((redirection = strstr(saved_input, ">")) != NULL) {
        *redirection = ' ';
        args[(*arg_count)++] = saved_input;
        //args[(*arg_count)++] = ">";
        args[(*arg_count)++] = redirection + 1;
        return;
    }
    
    char *delimiters = " ";
    // Use strsep to tokenize the input string
    while ((token = strsep(&input, delimiters)) != NULL) {
        if (*token != '\0') {
            args[(*arg_count)++] = token;
        }
    }// End while loop
    // Ensure the last element of args is NULL (for exec functions)
    args[*arg_count] = NULL;


}// End parse_input
//=================================================================================================================
// PATH command
void exe_path(char* args[]){
int i = 0;
    while (args[i] != NULL) {
        // Trim whitespaces before and after the directory
        char* dir = args[i];
        //while (isspace(*dir)) dir++;
        //char* end = dir + strlen(dir) - 1;
        //while (end > dir && isspace(*end)) end--;
        //*(end + 1) = '\0';

        path[i] = strdup(dir);
        i++;
    }
    path[i] = NULL;
}
//=================================================================================================================
// CD command
void exe_cd(char* args[]) {
    // Attempt to change the current directory
    if (chdir(args[1]) != 0) {
        // Signal an error for chdir failure
        //perror("chdir");
        write(STDERR_FILENO, error_message, strlen(error_message));
    }
    
}
//=================================================================================================================
void redirect_output(char* filename) {
    int output_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP);
    if (output_fd == -1) {
        perror("open");
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

//=================================================================================================================
