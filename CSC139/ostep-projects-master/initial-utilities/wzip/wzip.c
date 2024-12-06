#include <pthread.h>        //POSIX threads
#include <stdio.h>          //For fwrite
#include <fcntl.h>          //For opening files
#include <unistd.h>         //For closing files
#include <stdlib.h>         //For malloc and free
#include <sys/mman.h>       //For mmap
#include <sys/stat.h>       //For stat
#include <assert.h>         

// Holds thread arguments   
typedef struct myarg_t {
    char *char_ptr;  //points to the data we want to process
    int chunked; //portion of file that has already been chunked/starting position
    int chunk;  //the chunk of data that the thread will process
} myarg_t;

// Stores thread return values/what the thread generated
typedef struct myret_t {
    char   chars[999999];       //stores the character being processed
    int    char_counts[999999];     //stores how many of those characters were identified in a row
    int    n;   //the of elements in our arrays
} myret_t;

//Function Declarations
void pzip(char* curr, int* char_count, char *path, int isInitialThread);    //process used in main to invoke parallel zip
int getProcessorCount();    // method to get number of available cpus
void *threadRoutine(void *args);    //this is the thread routine (start_routine)
void combineResults(char* curr, int* char_count, myret_t* rvals[], int thread_count, int isInitialThread);    //method to combine the results generated from the threads

// MAIN
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("wzip: file1 [file2 ...]\n");
        exit(1);    
    }

    char curr;
    int char_count = 0;

    // First Process
    pzip(&curr, &char_count, argv[1], 1);

    // Subsequent processes
    for (int i = 2; i < argc; i++) {
        pzip(&curr, &char_count, argv[i], 0);
    }
    
    // write our results
    fwrite(&char_count, sizeof(int), 1 ,stdout);
    fwrite(&curr, sizeof(char), 1 ,stdout);
    return 0;
}

// PZIP
void pzip(char* curr, int* char_count, char *path, int isInitialThread) {
    int in_file = open(path, O_RDONLY);  //open file for reading
    if (in_file < 0) {
        printf("wzip: cannot open file\n");     //print error message
        exit(1);    //exit with error code 1
    }

    // Get info about the file
    struct stat file_info;     //stores the info
    int error = fstat(in_file, &file_info);
    if (error < 0) {
        printf("fstat failed\n");   //print error message
        exit(1);    //exit with error code 1
    }

    int filesize = file_info.st_size;      //get file size
    int chunked = 0;     //initialize chunked to 0

    //map the file into memory
    char *ptr = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, in_file, chunked); 
    if (ptr == MAP_FAILED) {
        printf("mmap failed\n");    //print error message
        exit(1);    //exit with error code 1
    }

    int thread_count = getProcessorCount();     //find out how many processors available
    int chunk = 0;     //initialize to 0
    pthread_t threads[thread_count];    //array of pthread_t structs to store thread IDs
    myarg_t args[thread_count];     //array of myarg_t structs to store thread arguments

    //loop to create threads
    for (int i = 0; i < thread_count; i++) {
        chunk = ((i+1) * filesize / thread_count) - chunked;     //the chunk of the file each thread will process; (increase chunk size after each chunk calculated)
        args[i] = (struct myarg_t){ptr, chunked, chunk};     //initialize the arguments for thread 'i'
        pthread_create(&threads[i], NULL, threadRoutine, &args[i]);     //create the thread
        chunked += chunk;   //update portion that has been chunked
    }

    myret_t* rvals[thread_count];   //points to the structs where we are storing the return values
    //loop to join threads and get their return values
    for (int i = 0; i < thread_count; i++) {
        pthread_join(threads[i], (void*)(&rvals[i]));
    }

    //combine the results
    combineResults(curr, char_count, rvals, thread_count, isInitialThread);

    //free memory
    for (int i = 0; i < thread_count; i++) {
        free(rvals[i]);
    }

    //unmap the file from memory
    error = munmap(ptr, filesize);
    if (error != 0) {
        printf("munmap failed\n");   //print error message
        exit(1);    //exit with error code 1
    }

    close(in_file);  //close the file
}

// GET PROCESSOR COUNT
#ifdef __linux__    //if linux OS
#include <sys/sysinfo.h>
// Method to get number of available CPUs on Linux
int getProcessorCount() {
    int count = get_nprocs();

    // Ensure count is at least 1
    if (count < 1) {
        count = 1;
    }

    return count;
}
#elif defined(__APPLE__)    //if macOS
#include <sys/sysctl.h>
// Method to get number of available CPUs on macOS
int getProcessorCount() {
    int info[2];
    size_t length = 4;
    uint32_t count;

    info[0] = CTL_HW;
    info[1] = HW_AVAILCPU;
    sysctl(info, 2, &count, &length, NULL, 0);

    // Ensure count is at least 1
    if (count < 1) {
        count = 1;
    }

    return count;
}
#else
#error "Unsupported operating system"
#endif

// THREAD ROUTINE
void *threadRoutine(void *args) {
    myret_t* rvals = malloc(sizeof(myret_t));   //allocate memory for return values
    rvals->n = 0;   //initialize the number of characters processed to 0

    //get thread arguments
    char *ptr = ((myarg_t *)args)->char_ptr;
    int chunked = ((myarg_t *)args)->chunked;
    int chunk = ((myarg_t *)args)->chunk;
    
    //initialize thr first character to be processed/set the starting position and initialize its count to 0
    char curr = *(ptr + chunked);
    int char_count = 0;

    //RLE 
    for (int i = 0; i < chunk; i++) {
        if (*(ptr + chunked + i) == curr) {
            char_count++;
        } else {
            rvals->chars[rvals->n] = curr;
            rvals->char_counts[rvals->n] = char_count;
            rvals->n++;
            char_count = 1;
            curr = *(ptr + chunked + i);
            assert(rvals->n < 999999);
        }
    }

    rvals->chars[rvals->n] = curr;
    rvals->char_counts[rvals->n] = char_count; 

    return (void*)rvals;
}

// COMBINE
void combineResults(char* curr, int* char_count, myret_t* rvals[], int thread_count, int isInitialThread) {
    //if we are trying to combine the results of the initial thread,
    //initialize the current character charcter and count
    if (isInitialThread == 1) {
        (*curr) = rvals[0]->chars[0];
        (*char_count) = 0; 
    }
    
    //combine results
    for (int i=0; i < thread_count; i++) {
        for (int j = 0; j <= rvals[i]->n; j++) {
            if ((*curr) == rvals[i]->chars[j]) {
                (*char_count) += rvals[i]->char_counts[j]; 
            } else {
                fwrite(char_count, sizeof(int), 1 ,stdout);
                fwrite(curr, sizeof(char), 1 ,stdout);
                (*curr) = rvals[i]->chars[j];
                (*char_count) = rvals[i]->char_counts[j];
            }
        }
    }
}
