#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("wzip: file1 [file2 ...]\n");
        return 1;
    }

    int count = 0;
    char curr, prev = '\0';

    for (int i = 1; i < argc; i++) {
        
        FILE *ip_file = fopen(argv[i], "r");

        if (ip_file == NULL) {
            printf("wzip: cannot open file\n");
            return 1;
        }

        while ((curr = fgetc(ip_file)) != EOF) {
            if (curr == prev) {
                count++;
            } else {
                if (count > 0) {
                    // Write count as a 4-byte binary integer
                    fwrite(&count, sizeof(int), 1, stdout);
                    // Write the character as a single ASCII character
                    fwrite(&prev, sizeof(char), 1, stdout);
                }
                prev = curr;
                count = 1;
            }
        }
        
        fclose(ip_file);
    }

    // Write count and character for the last sequence
    if (count > 0) {
        fwrite(&count, sizeof(int), 1, stdout);
        fwrite(&prev, sizeof(char), 1, stdout);
    }

    return 0;
}
