#!/bin/bash
# This script runs the pthread_program with perf stat for a given number of threads.
echo "I AM $1 Number of cores"
echo "I am using dan/625/wiki_dump.txt for input"
perf stat ./pthread_program ~dan/625/wiki_dump.txt $1
perf stat ./pthread_program ~dan/625/wiki_dump.txt $1
perf stat ./pthread_program ~dan/625/wiki_dump.txt $1
perf stat ./pthread_program ~dan/625/wiki_dump.txt $1
perf stat ./pthread_program ~dan/625/wiki_dump.txt $1