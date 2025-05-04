#!/bin/bash
echo "I AM $1 Number of cores"
echo "I am using dan/625/wiki_dump.txt for input"
perf stat mpirun $1 ./mpi_program ~dan/625/wiki_dump.txt
perf stat mpirun $1 ./mpi_program ~dan/625/wiki_dump.txt
perf stat mpirun $1 ./mpi_program ~dan/625/wiki_dump.txt
perf stat mpirun $1 ./mpi_program ~dan/625/wiki_dump.txt
perf stat mpirun $1 ./mpi_program ~dan/625/wiki_dump.txt