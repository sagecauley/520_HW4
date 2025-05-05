#!/bin/bash
echo "I AM $1 Number of cores"
echo "I am $2 for input"
perf stat ./openmp_program $2 $1
perf stat ./openmp_program $2 $1
perf stat ./openmp_program $2 $1
perf stat ./openmp_program $2 $1
perf stat ./openmp_program $2 $1