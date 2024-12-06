#include "umem.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>

#define MAX_HEADER_SIZE 32

// Node structure for the free list
typedef struct FreeNode {
    size_t size;
    struct FreeNode *next;
    int isAllocated;
} FreeNode;

// Structure to hold global state
typedef struct {
    FreeNode *freeList;
    void *memRegionStart;
    int allocationAlgorithm;
    FreeNode *lastAllocated;
} UMemState;

static UMemState umemState = {NULL, NULL, 0, NULL}; 

// Helper functions for umeminit
static void initializeMemoryStructures(size_t sizeOfRegion);     

// Helper functions for umalloc
static size_t alignTo8Bytes(size_t size);   
static FreeNode *findBestFit(size_t size); 
static FreeNode *findWorstFit(size_t size);
static FreeNode *findFirstFit(size_t size);
static FreeNode *findNextFit(size_t size);
// Helper functions for ufree     

// INIT
int umeminit(size_t sizeOfRegion, int allocationAlgo) {
    // Set the allocation algorithm
    umemState.allocationAlgorithm = allocationAlgo;
    // Check if umeminit has been called more than once and if algorithm is valid
    if (umemState.memRegionStart != NULL || sizeOfRegion <= 0) {
        fprintf(stderr, "umeminit failed: invalid parameters\n");
        return -1; // Failure
    }

    size_t pageSize = getpagesize();    // page size 
    // Round up size to the nearest page size
    sizeOfRegion = (sizeOfRegion + pageSize - 1) / pageSize * pageSize;
    // Check if sizeOfRegion is zero after rounding
    if (sizeOfRegion == 0) {
        fprintf(stderr, "umeminit failed: invalid sizeOfRegion after rounding\n");
        return -1; // Failure
    }
    // Allocate memory using mmap
    umemState.memRegionStart = mmap(NULL, sizeOfRegion, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    // Check if mmap was successful
    if (umemState.memRegionStart == MAP_FAILED) {
        umemState.memRegionStart = NULL;
        fprintf(stderr, "umeminit failed: mmap failed\n");
        return -1; // Failure
    } else {
        // Initialize other memory structures
        initializeMemoryStructures(sizeOfRegion);
    }
    return 0; // Success
}
//============================================================================================================
// Helper function to initialize memory structures based on the allocated memory region
static void initializeMemoryStructures(size_t sizeOfRegion) {
    // Initialize the free list
    umemState.freeList = (FreeNode*)umemState.memRegionStart;
    umemState.freeList->size = sizeOfRegion;
    umemState.freeList->next = NULL;
    umemState.freeList->isAllocated = 0;
}
//============================================================================================================
// UMALLOC
void *umalloc(size_t size) {
    if (size <= 0) {
        return NULL; // Invalid size
    }
    printf("Original Size: %zu\n", size);
    printf("Size of FreeNode structure: %zu\n", sizeof(FreeNode));
    size_t alignedSize = alignTo8Bytes(size+sizeof(FreeNode)); // Ensure 8-byte alignment
    printf("Aligned Size: %zu\n", alignedSize);

    FreeNode *selectedBlock = NULL;

    switch (umemState.allocationAlgorithm) {
        case BEST_FIT:
            selectedBlock = findBestFit(alignedSize);
            break;
        case WORST_FIT:
            selectedBlock = findWorstFit(alignedSize);
            break;
        case FIRST_FIT:
            selectedBlock = findFirstFit(alignedSize);
            break;
        case NEXT_FIT:
            selectedBlock = findNextFit(alignedSize);
            break;
        default:
            return NULL; // Unknown allocation algorithm
    }

    if (selectedBlock == NULL) {
        fprintf(stderr, "umalloc failed: no suitable block found\n");
        return NULL; // No suitable block found
    }

    // Update the free list
    if (selectedBlock->size > alignedSize + sizeof(FreeNode)) {
        // Split the block if it's larger than needed
        FreeNode *newFreeBlock = (FreeNode *)((char *)selectedBlock + alignedSize + sizeof(FreeNode));
        newFreeBlock->size = selectedBlock->size - (alignedSize + sizeof(FreeNode));
        newFreeBlock->next = selectedBlock->next;
        newFreeBlock->isAllocated = 0;

        selectedBlock->size = alignedSize;
        selectedBlock->next = newFreeBlock;
    }

    // Mark the selected block as allocated
    selectedBlock->isAllocated = 1;

    // Allocate memory from the selected block
    void *allocatedMemory = (void*)selectedBlock + sizeof(FreeNode);
    selectedBlock->size -= alignedSize;

    return allocatedMemory;
}
//============================================================================================================
// Function to align size to 8 bytes
size_t alignTo8Bytes(size_t size) {
    return (size + 7) & ~7;
}
//============================================================================================================
// Functions to implement allocation strategies
// BEST FIT
FreeNode *findBestFit(size_t size) {
    // BEST_FIT strategy
    FreeNode *currentBlock = umemState.freeList;
    FreeNode *bestFit = NULL;

    while (currentBlock != NULL) {
        if (currentBlock->size >= size) {
            if (bestFit == NULL || currentBlock->size < bestFit->size) {
                bestFit = currentBlock;
            }
        }
        currentBlock = currentBlock->next;
    }

    return bestFit;
}
//============================================================================================================
// WORST FIT
FreeNode *findWorstFit(size_t size) {
    // WORST_FIT strategy
    FreeNode *currentBlock = umemState.freeList;
    FreeNode *worstFit = NULL;

    while (currentBlock != NULL) {
        if (currentBlock->size >= size) {
            if (worstFit == NULL || currentBlock->size > worstFit->size) {
                worstFit = currentBlock;
            }
        }
        currentBlock = currentBlock->next;
    }

    return worstFit;
}
//============================================================================================================
//FIRST FIT
FreeNode *findFirstFit(size_t size) {
    // FIRST_FIT strategy
    FreeNode *currentBlock = umemState.freeList;

    while (currentBlock != NULL) {
        if (currentBlock->size >= size) {
            return currentBlock; // Found a block large enough
        }
        currentBlock = currentBlock->next;
    }

    return NULL; // No suitable block found
}
//============================================================================================================
// NEXT FIT
FreeNode *findNextFit(size_t size) {
    // NEXT_FIT strategy
    FreeNode *currentBlock = umemState.lastAllocated;

    while (currentBlock != NULL) {
        if (currentBlock->size >= size) {
            return currentBlock; // Found a block large enough
        }
        currentBlock = currentBlock->next;
    }

    // If no suitable block is found from lastFitPosition to the end, wrap around to the beginning
    currentBlock = umemState.freeList;
    while (currentBlock != umemState.lastAllocated) {
        if (currentBlock->size >= size) {
            return currentBlock; // Found a block large enough
        }
        currentBlock = currentBlock->next;
    }

    return NULL; // No suitable block found
}
//============================================================================================================
// UFREE
int ufree(void *ptr) {
    if (ptr == NULL) {
        return -1; // Invalid pointer
    }

    // Calculate the block to free
    FreeNode *blockToFree = (FreeNode*)((char*)ptr - sizeof(FreeNode));

    // Mark the block as free
    blockToFree->isAllocated = 0;

    // Coalesce with the previous block
    FreeNode *prevBlock = umemState.freeList;
    while (prevBlock != NULL && prevBlock->next != blockToFree) {
        prevBlock = prevBlock->next;
    }

    if (prevBlock != NULL && !prevBlock->isAllocated) {
        prevBlock->size += sizeof(FreeNode) + blockToFree->size;
        prevBlock->next = blockToFree->next;
        blockToFree = prevBlock;
    }

    // Coalesce with the next block
    FreeNode *nextBlock = blockToFree->next;
    if (nextBlock != NULL && !nextBlock->isAllocated) {
        blockToFree->size += sizeof(FreeNode) + nextBlock->size;
        blockToFree->next = nextBlock->next;
    }

    // Update lastAllocated for NEXT_FIT
    umemState.lastAllocated = blockToFree;

    return 0; // Success
}
//============================================================================================================
// UMEDUMP
void umemdump() {
    printf("Memory Dump:\n");

    FreeNode *currentBlock = umemState.freeList;
    while (currentBlock != NULL) {
        printf("Block: size=%zu, address=%p, isAllocated=%d\n",
               currentBlock->size, (void*)currentBlock, currentBlock->isAllocated);
        currentBlock = currentBlock->next;
    }

    printf("\n");
}
