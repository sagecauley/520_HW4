#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024

// Function to compute max ASCII value in a line (up to '\n' or null terminator)
int max_ascii_value_mpi(const char* line) {
    int max_value = 0;
    for (int i = 0; i < MAX_LINE_LENGTH && line[i] != '\0' && line[i] != '\n'; ++i) {
        unsigned char c = (unsigned char)line[i];
        if (c <= 127 && c > max_value) {
            max_value = c;
        }
    }
    return max_value;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <file_path> <num_threads>\n", argv[0]);
        return 1;
    }

    const char* file_path = argv[1];
    int num_threads = atoi(argv[2]);

    clock_t start_time = clock();

    MPI_Init(&argc, &argv);

    int pid, num_procs;
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    if (num_threads != num_procs) {
        if (pid == 0) {
            fprintf(stderr, "Error: Number of threads (MPI processes) must match num_threads argument.\n");
        }
        MPI_Finalize();
        return 1;
    }

    char (*lines)[MAX_LINE_LENGTH] = NULL;
    int num_lines = 0;

    if (pid == 0) {
        FILE* file = fopen(file_path, "r");
        if (!file) {
            fprintf(stderr, "Error opening file: %s\n", file_path);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        size_t capacity = 1024;
        lines = malloc(capacity * MAX_LINE_LENGTH);
        if (!lines) {
            fprintf(stderr, "Initial memory allocation failed\n");
            fclose(file);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        char buffer[MAX_LINE_LENGTH];
        while (fgets(buffer, MAX_LINE_LENGTH, file)) {
            if (num_lines >= capacity) {
                capacity *= 2;
                char (*new_lines)[MAX_LINE_LENGTH] = realloc(lines, capacity * MAX_LINE_LENGTH);
                if (!new_lines) {
                    fprintf(stderr, "Memory reallocation failed\n");
                    free(lines);
                    fclose(file);
                    MPI_Abort(MPI_COMM_WORLD, 1);
                }
                lines = new_lines;
            }

            // Copy raw memory instead of using strncpy
            for (int i = 0; i < MAX_LINE_LENGTH; ++i) {
                lines[num_lines][i] = buffer[i];
                if (buffer[i] == '\0') break;
            }
            num_lines++;
        }

        fclose(file);
    }

    // Broadcast the number of lines
    MPI_Bcast(&num_lines, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Allocate local storage
    int lines_per_proc = (num_lines + num_procs - 1) / num_procs;
    char* local_lines = malloc(lines_per_proc * MAX_LINE_LENGTH);
    int* local_max_ascii = malloc(lines_per_proc * sizeof(int));

    // Scatter the data
    MPI_Scatter(lines, lines_per_proc * MAX_LINE_LENGTH, MPI_CHAR,
                local_lines, lines_per_proc * MAX_LINE_LENGTH, MPI_CHAR,
                0, MPI_COMM_WORLD);

    // Compute local max ASCII values
    for (int i = 0; i < lines_per_proc; ++i) {
        local_max_ascii[i] = max_ascii_value_mpi(&local_lines[i * MAX_LINE_LENGTH]);
    }

    int* all_max_ascii = NULL;
    if (pid == 0) {
        all_max_ascii = malloc(num_lines * sizeof(int));
    }

    // Gather all results to the root process
    MPI_Gather(local_max_ascii, lines_per_proc, MPI_INT,
               all_max_ascii, lines_per_proc, MPI_INT,
               0, MPI_COMM_WORLD);

    if (pid == 0) {
        for (int i = 0; i < num_lines; ++i) {
            printf("Line %d, Max ASCII: %d\n", i, all_max_ascii[i]);
        }

        free(all_max_ascii);
    }

    // Free allocated memory
    free(local_lines);
    free(local_max_ascii);

    MPI_Finalize();

    return 0;
}
