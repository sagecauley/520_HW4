#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int NUM_THREADS;



pthread_mutex_t count_mutex;
char** lines = NULL;            // Array of dynamically allocated lines
int num_lines = 0;              // Total number of lines initally 0 allocates it in main
int* max_char_values = NULL;

pthread_t* threads;
int* thread_ids;

// Function to compute max ASCII value in a line
int max_ascii_value(const char* line) {
    int max_value = 0;
    //Goes through each character
    while (*line) {
        unsigned char c = (unsigned char)*line;
        if (c <= 127 && c > max_value) {
            max_value = c; // set new max value if current char exceeds previous max
        }
        line++;
    }
    return max_value;
}

// Thread function to process a portion of lines
void* process_lines(void* arg) {
    int thread_id = *((int*)arg);
    int lines_per_thread = num_lines / NUM_THREADS;
    int start_line = thread_id * lines_per_thread;
    int end_line;

    if (thread_id == NUM_THREADS - 1) {
        end_line = num_lines; // if final thread, set end line to last line
    } else {
        end_line = start_line + lines_per_thread;
        // otherwise, calculate using start line and # of lines per thread
    }

    //Getting the max value from everyline
    for (int i = start_line; i < end_line; i++) {
        int max_value = max_ascii_value(lines[i]);

        pthread_mutex_lock(&count_mutex); // lock access to max_char_values
        max_char_values[i] = max_value; // set max value for location i
        pthread_mutex_unlock(&count_mutex); // return access to max_char_values
    }

    return NULL;
}

//Main function to run and print out the max ASCII character in each line
int main(int argc, char* argv[]) {
    if (argc != 3) {
        // print error if improper arguments passed
        fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
        return -1;
    }

    const char* file_path = argv[1]; // set file path from args
    NUM_THREADS = atoi(argv[2]); // set number of threads from args
    FILE* file = fopen(file_path, "r"); // open file with lines
    if (!file) {
        printf("Error opening file"); // print error if couldn't open file
        return -1;
    }

    // set up the lines
    size_t capacity = 1000000;
    lines = malloc(capacity * sizeof(char*)); // allocate memory for lines
    if (!lines) {
        printf("malloc failed");
        fclose(file); // throw error if failure allocation
        return -1;
    }

    char* line = NULL;
    size_t len = 0;
    //Reads in each line and sets it
    while (getline(&line, &len, file) != -1) {
        //Allocates more memory if the lines go above 1 million
        if (num_lines >= capacity) {
            capacity *= 2; // increase capacity if num_lines exceeds capacity
            lines = realloc(lines, capacity * sizeof(char*));
            if (!lines) {
                printf("realloc failed");
                fclose(file); // throw error if reallocation fails
                free(line);
                return -1;
            }
        }
        // Allocate memory and copy the line
        lines[num_lines] = malloc(len + 1);
        if (!lines[num_lines]) {
            printf("malloc failed for line");
            fclose(file); // throw error if malloc fails
            free(line);
            return -1;
        }
        memcpy(lines[num_lines], line, len + 1);  // Copy the line including the null terminator
        
        num_lines++;
    }
    free(line);
    fclose(file);

    // Allocate memory for results
    max_char_values = malloc(num_lines * sizeof(int));
    if (!max_char_values) {
        printf("malloc failed");
        return -1;
    }

    threads = malloc(NUM_THREADS * sizeof(pthread_t));
    thread_ids = malloc(NUM_THREADS * sizeof(int));

    pthread_mutex_init(&count_mutex, NULL);

    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);


    
    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, process_lines, &thread_ids[i]);
    }
    


    // Wait for all threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    // Record the end time
    clock_gettime(CLOCK_MONOTONIC, &end_time);

    // Calculate the elapsed time in seconds
    double elapsed_time = (end_time.tv_sec - start_time.tv_sec) +
    (end_time.tv_nsec - start_time.tv_nsec) / 1e9;

    // Print the elapsed time
    printf("Thread time: %f seconds\n", elapsed_time);
    // Print results
    for (int i = 0; i < num_lines; i++) {
        //printf("%d: %d\n", i, max_char_values[i]);
    }

    // Clean up
    for (int i = 0; i < num_lines; i++) {
        free(lines[i]);
    }
    free(lines);
    free(max_char_values);
    free(threads);
    free(thread_ids);
    pthread_mutex_destroy(&count_mutex); // destroy mutex after finished


    return 0;
}
