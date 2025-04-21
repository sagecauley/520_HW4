#!/bin/sh
sbatch --time=10 --mem=7G --cpus-per-task=1 --threads-per-core=1 --ntasks=1 --nodes=1 --constraint=moles --job-name='1_core' ./program_thread.sh 1
sbatch --time=5 --mem=7G --cpus-per-task=5 --threads-per-core=1 --ntasks=1 --nodes=1 --constraint=moles --job-name='5_core' ./program_thread.sh 5
sbatch --time=5 --mem=7G --cpus-per-task=10 --threads-per-core=1 --ntasks=1 --nodes=1 --constraint=moles --job-name='10_core' ./program_thread.sh 10
sbatch --time=5 --mem=7G --cpus-per-task=15 --threads-per-core=1 --ntasks=1 --nodes=1 --constraint=moles --job-name='15_core' ./program_thread.sh 15
sbatch --time=5 --mem=7G --cpus-per-task=20 --threads-per-core=1 --ntasks=1 --nodes=1 --constraint=moles --job-name='20_core' ./program_thread.sh 20

#Run on 2 diffrent system
sbatch --time=5 --mem=7G --cpus-per-task=10 --threads-per-core=1 --ntasks=2 --nodes=2 --constraint=moles --job-name='20_core_2sys' ./program_thread.sh 20


#This Will run diffrent files for the program 
#If you want to run the diffrent files for the program you need to let me know as the files are not in the repo and are too large to upload
sbatch --time=5 --mem=7G --cpus-per-task=10 --threads-per-core=1 --ntasks=2 --nodes=2 --constraint=moles --job-name='5_core_1M' ./pthread_dif_files.sh 5 random_text_1k.txt
sbatch --time=5 --mem=7G --cpus-per-task=10 --threads-per-core=1 --ntasks=2 --nodes=2 --constraint=moles --job-name='5_core_100K' ./pthread_dif_files.sh 5 random_text_10k.txt
sbatch --time=5 --mem=7G --cpus-per-task=10 --threads-per-core=1 --ntasks=2 --nodes=2 --constraint=moles --job-name='5_core_10K' ./pthread_dif_files.sh 5 random_text_100k.txt
sbatch --time=5 --mem=7G --cpus-per-task=10 --threads-per-core=1 --ntasks=2 --nodes=2 --constraint=moles --job-name='5_core_1K' ./pthread_dif_files.sh 5 random_text_1M.txt





#sbatch --time=5 --mem-per-cpu=512M --cpus-per-task=10 --ntasks=2 --nodes=1 --constraint=moles ./program_thread.sh
