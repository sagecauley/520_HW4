#!/bin/sh
sbatch --time=30 --mem=7G --cpus-per-task=1 --threads-per-core=1 --ntasks=1 --nodes=1 --constraint=moles --job-name='1_core' ./program_thread.sh
sbatch --time=30 --mem=7G --cpus-per-task=5 --threads-per-core=1 --ntasks=1 --nodes=1 --constraint=moles --job-name='5_core' ./program_thread.sh
sbatch --time=5 --mem=7G --cpus-per-task=10 --threads-per-core=1 --ntasks=1 --nodes=1 --constraint=moles --job-name='10_core' ./program_thread.sh
sbatch --time=5 --mem=7G --cpus-per-task=15 --threads-per-core=1 --ntasks=1 --nodes=1 --constraint=moles --job-name='15_core' ./program_thread.sh
sbatch --time=5 --mem=7G --cpus-per-task=20 --threads-per-core=1 --ntasks=1 --nodes=1 --constraint=moles --job-name='20_core' ./program_thread.sh
sbatch --time=2 --mem=7G --cpus-per-task=10 --threads-per-core=1 --ntasks=2 --nodes=2 --constraint=moles --job-name='20_core_2systems' ./program_thread.sh






#sbatch --time=5 --mem-per-cpu=512M --cpus-per-task=10 --ntasks=2 --nodes=1 --constraint=moles ./program_thread.sh
