
#include <mpi.h>
#include <pthread.h> // DELETE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_THREADS 40


pthread_mutex_t count_mutex;
char** lines = NULL;            // Array of dynamically allocated lines
int num_lines = 0;              // Total number of lines initally 0 allocates it in main

pthread_t threads[NUM_THREADS];
int thread_ids[NUM_THREADS];


// Function to compute max ASCII value in a line
int max_ascii_value_mpi(const char* line) {
    int max_value = 0;
    //Goes through each character
    while (*line) {
        unsigned char c = (unsigned char)*line;
        if (c <= 127 && c > max_value) {
            max_value = c;
        }
        line++;
    }
    return max_value;
}

void process_lines_mpi(char *threadData, int dataSplitSize, int maxLength, int maxCharValues[] ) {
    for (int i = 0; i < dataSplitSize / sizeof(char); i += maxLength) {
	char* line = malloc(sizeof(char)*maxLength);
	strncpy(line, (char*)threadData[i], maxLength);
        maxCharValues[i] = max_ascii_value_mpi(threadData[i]);
    }
}

//Main function to run and print out the max ASCII character in each line
int main(int argc, char* argv[]) {
    int maxLength = 0;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
        return -1;
    }

    const char* file_path = argv[1];
    FILE* file = fopen(file_path, "r");
    if (!file) {
        printf("Error opening file");
        return -1;
    }

    // set up the lines
    size_t capacity = 1000000;
    lines = malloc(capacity * sizeof(char*));
    if (!lines) {
        printf("malloc failed");
        fclose(file);
        return -1;
    }

    char* line = NULL;
    size_t len = 0;
    //Reads in each line and sets it
    while (getline(&line, &len, file) != -1) {
        //Allocates more memory if the lines go above 1 million
        if (num_lines >= capacity) {
            capacity *= 2;
            lines = realloc(lines, capacity * sizeof(char*));
            if (!lines) {
                printf("realloc failed");
                fclose(file);
                free(line);
                return -1;
            }
        }
        // Allocate memory and copy the line
        lines[num_lines] = malloc(len + 1);  // +1 for the '/0'
        if (!lines[num_lines]) {
            printf("malloc failed for line");
            fclose(file);
            free(line);
            return -1;
        }
        memcpy(lines[num_lines], line, len + 1);  // Copy the line including the null terminator
        
        num_lines++;

        if (len + 1 > maxLength) { // set max length
	    maxLength = len + 1;
	}
    }
    free(line);
    fclose(file);

    printf("DBG0\n");

    // Record the start time
    clock_t start_time = clock();

    // https://hpc.nmsu.edu/discovery/mpi/programming-with-mpi/
    MPI_Init(NULL, NULL);

    printf("DBG2\n");

    int pid;
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);

    int processCount;
    MPI_Comm_size(MPI_COMM_WORLD, &processCount);

    int dataSplitSize = num_lines / NUM_THREADS;
    char threadData[dataSplitSize][maxLength];

    printf("DBG3\n");

    MPI_Scatter(fixedSizeLines, dataSplitSize, MPI_INT, threadData, dataSplitSize, MPI_INT, 0, MPI_COMM_WORLD); // Distribute memory

    int maxCharValues[dataSplitSize];

    process_lines_mpi(threadData, dataSplitSize, maxLength, maxCharValues);

    char finalResults[num_lines][maxLength];

    if(pid == 0) {
        MPI_Gather(threadData, dataSplitSize, MPI_INT, finalResults, dataSplitSize, MPI_INT, 0, MPI_COMM_WORLD);
    } else {
        MPI_Gather(threadData, dataSplitSize, MPI_INT, NULL, dataSplitSize, MPI_INT, 0, MPI_COMM_WORLD);
    }

    if(pid == 0) { // if rank 0
	printf("ARRAY:\n");
	for (int i = 0; i < num_lines; i++) {
	    printf("%d ", finalResults[i]);
	}
    }

    MPI_Finalize();

    // Record the end time
    clock_t end_time = clock();

    // Calculate the elapsed time in seconds
    double elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    // Print the elapsed time
    printf("Thread time: %f seconds\n", elapsed_time);
    // Print results

    // Clean up
    for (int i = 0; i < num_lines; i++) {
        free(lines[i]);
    }
    free(lines);
    //printf("Task_Complete");

    return 0;
}
