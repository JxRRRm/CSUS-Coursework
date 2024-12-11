#include <stdio.h>  // standard input output
#include <stdlib.h> // standard library

int main(int argc, char *argv[]) {
    // Check if there are no files specified on the command line
    if (argc < 2) {
        printf("wunzip: file1 [file2 ...]\n");
        return 1; // Exit with status code 1
    }

    for (int i = 1; i < argc; i++) {
        FILE *file = fopen(argv[i], "rb"); // Open in binary mode for reading
        if (file == NULL) {
            perror("wunzip: cannot open file\n");
            return 1;
        }

        int count;
        char curr;

        while (fread(&count, sizeof(count), 1, file) == 1) {
            fread(&curr, sizeof(curr), 1, file);
            for (int j = 0; j < count; j++) {
                fprintf(stdout, "%c", curr); // Use fprintf to write to stdout
            }
        }

        fclose(file);
    }

    return 0;
}
