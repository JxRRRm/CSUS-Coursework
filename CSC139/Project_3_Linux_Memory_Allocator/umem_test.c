#include "umem.c"
#include <stdio.h>
#include <unistd.h>

int main() {
    // Initialize memory allocator
    if (umeminit(getpagesize(), BEST_FIT) != 0) {
        fprintf(stderr, "Memory allocator initialization failed.\n");
        return -1;
    }

    // Allocate memory using different strategies
    void *ptr1 = umalloc(2);
    void *ptr2 = umalloc(20);
    void *ptr3 = umalloc(64);

    // Print allocated memory pointers
    printf("Pointer 1: %p\n", ptr1);
    printf("Pointer 2: %p\n", ptr2);
    printf("Pointer 3: %p\n", ptr3);

    // Dump memory to check free blocks
    umemdump();

    // Free allocated memory
    ufree(ptr1);
    ufree(ptr2);
    ufree(ptr3);

    // Dump memory after freeing to check coalescing
    umemdump();

    return 0;
}
