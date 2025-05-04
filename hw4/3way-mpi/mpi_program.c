#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
        return 1;
    }

    const char* file_path = argv[1];

    clock_t start_time = clock();

    MPI_Init(&argc, &argv);

    int pid, num_procs;
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    char (*lines)[MAX_LINE_LENGTH] = NULL;
    int num_lines = 0;

    if (pid == 0) {
        FILE* file = fopen(file_path, "r");
        if (!file) {
            fprintf(stderr, "Error opening file: %s\n", file_path);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        size_t capacity = 1024;
        lines = malloc(capacity * sizeof(*lines));
        if (!lines) {
            fprintf(stderr, "Initial memory allocation failed\n");
            fclose(file);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        char buffer[MAX_LINE_LENGTH];
        while (fgets(buffer, MAX_LINE_LENGTH, file)) {
            if (num_lines >= capacity) {
                capacity *= 2;
                char (*new_lines)[MAX_LINE_LENGTH] = realloc(lines, capacity * sizeof(*lines));
                if (!new_lines) {
                    fprintf(stderr, "Memory reallocation failed\n");
                    free(lines);
                    fclose(file);
                    MPI_Abort(MPI_COMM_WORLD, 1);
                }
                lines = new_lines;
            }

            strncpy(lines[num_lines], buffer, MAX_LINE_LENGTH);
            num_lines++;
        }

        fclose(file);
    }

    // Broadcast number of lines to all processes
    MPI_Bcast(&num_lines, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Compute send counts and displacements (in bytes)
    int* sendcounts = NULL;
    int* displs = NULL;

    if (pid == 0) {
        sendcounts = malloc(num_procs * sizeof(int));
        displs = malloc(num_procs * sizeof(int));

        int base = num_lines / num_procs;
        int rem = num_lines % num_procs;

        for (int i = 0; i < num_procs; ++i) {
            int count = (i < rem ? base + 1 : base);
            sendcounts[i] = count * MAX_LINE_LENGTH;
            displs[i] = (i == 0) ? 0 : displs[i - 1] + sendcounts[i - 1];
        }
    }

    // Each process calculates how many lines it will get
    int local_line_bytes;
    MPI_Scatter(sendcounts, 1, MPI_INT, &local_line_bytes, 1, MPI_INT, 0, MPI_COMM_WORLD);
    int local_line_count = local_line_bytes / MAX_LINE_LENGTH;

    char* local_lines = malloc(local_line_bytes);
    int* local_max_ascii = malloc(local_line_count * sizeof(int));

    // Scatterv lines to all processes
    MPI_Scatterv(pid == 0 ? lines : NULL, sendcounts, displs, MPI_CHAR,
                 local_lines, local_line_bytes, MPI_CHAR,
                 0, MPI_COMM_WORLD);

    // Compute local max ASCII values
    for (int i = 0; i < local_line_count; ++i) {
        local_max_ascii[i] = max_ascii_value_mpi(&local_lines[i * MAX_LINE_LENGTH]);
    }

    // Compute recvcounts and displs for gathering INTs (not bytes)
    int* recvcounts_int = NULL;
    int* displs_int = NULL;
    if (pid == 0) {
        recvcounts_int = malloc(num_procs * sizeof(int));
        displs_int = malloc(num_procs * sizeof(int));
        for (int i = 0; i < num_procs; ++i) {
            recvcounts_int[i] = sendcounts[i] / MAX_LINE_LENGTH;
            displs_int[i] = (i == 0) ? 0 : displs_int[i - 1] + recvcounts_int[i - 1];
        }
    }

    int* all_max_ascii = NULL;
    if (pid == 0) {
        all_max_ascii = malloc(num_lines * sizeof(int));
    }

    // Gather all max ASCII values to root
    MPI_Gatherv(local_max_ascii, local_line_count, MPI_INT,
                all_max_ascii, recvcounts_int, displs_int, MPI_INT,
                0, MPI_COMM_WORLD);

    if (pid == 0) {
        for (int i = 0; i < num_lines; ++i) {
            printf("Line %d max ASCII: %d\n", i, all_max_ascii[i]);
        }

        free(all_max_ascii);
        free(lines);
        free(sendcounts);
        free(displs);
        free(recvcounts_int);
        free(displs_int);
    }

    free(local_lines);
    free(local_max_ascii);

    MPI_Finalize();

    return 0;
}
