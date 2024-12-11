#include <stdio.h>  // standard input output
#include <stdlib.h> // standard library
#include <string.h> // strings


int main(int argc, char *argv[]) {
    // Check for the correct number of command-line arguments
    if (argc < 2) {
        printf("wgrep: searchterm [file ...]\n");
        return 1;
    }
    // variables
    const char *search_str = argv[1];               // search string pointer
    char *line = (char *)malloc(2048);              // Allocate memory for the line buffer;                              // line pointer
    size_t line_size = 0;                           // length of line (initally 0)
    const int search_str_len = strlen(search_str);  // length of search term

    // Check if there are files specified
    if (argc == 2) {
         // Search from standard input using fgets (no file specified)
        while (fgets(line, 2048, stdin) != NULL) {
            if (strstr(line, search_str) != NULL) {
                printf("%s", line);
            }
        }
    } else if (argc > 2) {
        // Loop through each file specified on the command line
        for (int i = 2; i < argc; i++) {
            FILE *file = fopen(argv[i], "r");

            // handle the case where wgrep cannot open file
            if (file == NULL) {
                printf("wgrep: cannot open file\n");
                return 1;
            }// end if

            while (getline(&line, &line_size, file) != -1) {    // if == -1, end of file is reached
                // if search term is an empty string or the search string is found in the current line
                if (search_str_len == 0 || strstr(line, search_str) != NULL) {
                    printf("%s", line); // print the current line
                }// end if
            }// end while

            fclose(file);   // close file
        }// end for
    }// end if-else

    free(line); // deallocate memory

    return 0;
}
