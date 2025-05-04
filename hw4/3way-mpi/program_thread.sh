#!/bin/bash
echo "I AM $1 Number of cores"
echo "I am using dan/625/wiki_dump.txt for input"
perf stat mpirun --use-hwthread-cpus ./mpi_program ~dan/625/wiki_dump.txt $1
#perf stat mpirun --use-hwthread-cpus ./mpi_program ~dan/625/wiki_dump.txt $1
#perf stat mpirun --use-hwthread-cpus ./mpi_program ~dan/625/wiki_dump.txt $1
#perf stat mpirun --use-hwthread-cpus ./mpi_program ~dan/625/wiki_dump.txt $1
#perf stat mpirun --use-hwthread-cpus ./mpi_program ~dan/625/wiki_dump.txt $1