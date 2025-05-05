#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>

int NUM_THREADS;

char** lines = NULL;            // Array of dynamically allocated lines
int num_lines = 0;              // Total number of lines
int* max_char_values = NULL;

// Function to compute max ASCII value in a line
int max_ascii_value(const char* line) {
    int max_value = 0;
    while (*line) { // while there are more chars left
        unsigned char c = (unsigned char)*line;
        if (c <= 127 && c > max_value) {
            max_value = c; // update max value if current char exceeds
        }
        line++;
    }
    return max_value;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        // if number of args is incorrect, throw error.
        fprintf(stderr, "Usage: %s <file_path> <num_threads>\n", argv[0]);
        return -1;
    }

    const char* file_path = argv[1];
    NUM_THREADS = atoi(argv[2]);
    FILE* file = fopen(file_path, "r"); // open file with lines
    if (!file) {
        printf("Error opening file\n"); // throw error if opening fails
        return -1;
    }

    // Allocate memory for lines
    size_t capacity = 1000000;
    lines = malloc(capacity * sizeof(char*)); 
    if (!lines) {
        printf("malloc failed\n"); // throw error if malloc fails
        fclose(file);
        return -1;
    }

    char* line = NULL;
    size_t len = 0;
    while (getline(&line, &len, file) != -1) { // get each line from file
        if (num_lines >= capacity) {
            capacity *= 2; // increase capacity if there is not enough space for lines
            lines = realloc(lines, capacity * sizeof(char*));
            if (!lines) {
                printf("realloc failed\n");
                fclose(file);
                free(line); // throw error if reallocation fails
                return -1;
            }
        }
        lines[num_lines] = malloc(len + 1); // allocate space for line and string terminator
        if (!lines[num_lines]) {
            printf("malloc failed for line\n");
            fclose(file);
            free(line); // throw error if allocation fails
            return -1;
        }
        memcpy(lines[num_lines], line, len + 1);
        num_lines++;
    }
    free(line);
    fclose(file);

    // Allocate memory for results
    max_char_values = malloc(num_lines * sizeof(int));
    if (!max_char_values) {
        printf("malloc failed\n");
        return -1;
    }

    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    // Parallel processing using OpenMP
    omp_set_num_threads(NUM_THREADS);
    #pragma omp parallel for
    for (int i = 0; i < num_lines; i++) {
        max_char_values[i] = max_ascii_value(lines[i]);
    }

    clock_gettime(CLOCK_MONOTONIC, &end_time);

    double elapsed_time = (end_time.tv_sec - start_time.tv_sec) +
                          (end_time.tv_nsec - start_time.tv_nsec) / 1e9;

    printf("OpenMP time: %f seconds\n", elapsed_time);

    // Uncomment if you want to print the results
    for (int i = 0; i < 200; i++) {
        printf("%d: %d\n", i, max_char_values[i]);
    }

    // Clean up
    for (int i = 0; i < num_lines; i++) {
        free(lines[i]);
    }
    free(lines);
    free(max_char_values);

    return 0;
}
