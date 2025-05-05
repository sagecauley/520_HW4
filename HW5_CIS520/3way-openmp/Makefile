#
# Open Systems Lab
# http://www.lam-mpi.org/tutorials/
# Indiana University
#
# MPI Tutorial
# Sample Makefile
#

# "mpicc" adds the directories for the include and lib files.  Hence,
# -I and -L for MPI stuff is not necessary
#

CC        = gcc
CFLAGS = -fopenmp -O2

#
# Modify TOPDIR if you use your own include files and library files
#

PROGRAM   = openmp_program		# name of the binary
SRCS      = openmp_program.c		# source file
OBJS      = $(SRCS:.c=.o)		# object file

#
# Targets
#

default: all

all: $(PROGRAM) 

$(PROGRAM): $(OBJS)
	$(CC) $(OBJS) -o $(PROGRAM) $(CFLAGS)

clean:
	/bin/rm -f $(OBJS) $(PROGRAM)
