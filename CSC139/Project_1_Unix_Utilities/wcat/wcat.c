#include <stdio.h>  // standard input output
#include <stdlib.h> // standard library

int main(int argc, char *argv[]) {
    // Check if there are no files specified on the command line
    if (argc < 2) {
        return 0; // Exit with status code 0
    }

    // Loop through each file specified on the command line
    for (int i = 1; i < argc; i++) {
        FILE *file = fopen(argv[i], "r");   // file pointer

        if (file == NULL) {
            // Print an error message and exit with status code 1
            printf("wcat: cannot open file\n");
            return 1;
        }

        // Read and print the contents of the file line by line
        char buffer[256]; 
        while (fgets(buffer, sizeof(buffer), file) != NULL) {
            printf("%s", buffer);
        }

        // Close the file
        fclose(file);
    }

    return 0; // Exit with status code 0
}

