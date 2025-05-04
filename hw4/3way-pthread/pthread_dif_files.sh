#!/bin/bash
echo "I AM $1 Number of cores"
echo "I am $2 for input"
perf stat ./pthread_program $2 $1
perf stat ./pthread_program $2 $1
perf stat ./pthread_program $2 $1
perf stat ./pthread_program $2 $1
perf stat ./pthread_program $2 $1