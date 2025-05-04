#!/bin/bash
echo "I AM $1 Number of cores"
echo "I am $2 for input"
perf stat mpirun $1  ./mpi_program $2
perf stat mpirun $1  ./mpi_program $2
perf stat mpirun $1  ./mpi_program $2
perf stat mpirun $1  ./mpi_program $2
perf stat mpirun $1  ./mpi_program $2