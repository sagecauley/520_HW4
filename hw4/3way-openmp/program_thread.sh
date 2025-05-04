#!/bin/bash
echo "I AM $1 Number of cores"
echo "I am using dan/625/wiki_dump.txt for input"
perf stat ./openmp_program ~dan/625/wiki_dump.txt $1
perf stat ./openmp_program ~dan/625/wiki_dump.txt $1
perf stat ./openmp_program ~dan/625/wiki_dump.txt $1
perf stat ./openmp_program ~dan/625/wiki_dump.txt $1
perf stat ./openmp_program ~dan/625/wiki_dump.txt $1