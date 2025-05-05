#!/bin/bash

#This script is to qucickly set up the environment for running the programs 
# I am lazy to do it manually

chmod +x 3way-pthread/configs.sh
chmod +x 3way-pthread/program_thread.sh
chmod +x 3way-pthread/pthread_dif_files.sh

#go into the 3way-pthread directory
cd 3way-pthread
#make the program
#make clean
make
#run the configs.sh file
./configs.sh
#exit
#go back to the main directory
cd ..

chmod +x 3way-mpi/configs.sh
chmod +x 3way-mpi/program_thread.sh
chmod +x 3way-mpi/pthread_dif_files.sh
#go into the 3way-mpi directory
cd 3way-mpi
#make the program
#make clean
make
#run the configs.sh file
./configs.sh
#exit
#go back to the main directory
cd ..

chmod +x 3way-openmp/configs.sh
chmod +x 3way-openmp/program_thread.sh
chmod +x 3way-openmp/pthread_dif_files.sh

#go into the 3way-openmp directory
cd 3way-openmp
#make the program
#make clean
make
#run the configs.sh file
./configs.sh
#exit
#go back to the main directory
cd ..
