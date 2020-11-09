#!/bin/bash

PARTIAL_PARALLEL_FILE=./paralelo/partial.o
FULL_PARALLEL_FILE=./paralelo/full.o
SEQUENTIAL_FILE=./sequencial/radix_seq.o


run(){
    for input in 10000000 50000000 100000000
        do
            echo "Running sequential algorithm... with = $input elements"
            START=$(date +%s.%N)
            for ((i=1;i<=times;i++)); do
                $SEQUENTIAL_FILE -n $input -d 3 
            done
            END=$(date +%s.%N)
            TIME_SEQ=$(python3 -c "print('{:.2f}'.format(${END} - ${START}))")
            
         echo "Running paralell algorithm with $threads threads... with = $input elements" 
         START=$(date +%s.%N)
         for ((i=1;i<=times;i++)); do
            $PARALLEL_FILE -n $input -d 3 -t $threads
         done
         END=$(date +%s.%N)
         TIME_PAR=$(python3 -c "print('{:.2f}'.format(${END} - ${START}))")
         SPEEDUP=$(python3 -c "print('{:.2f}'.format(${TIME_SEQ} / ${TIME_PAR}))")
         echo "Speed up for input $input and $threads threads is $SPEEDUP"
         echo ""
        done
}

helpFunction()
{
   echo ""
   echo "Usage: $0 -n "
   echo -e "\t-n number of times that the programas will run "
   echo -e "\t-t number of threads "
}

while getopts "n:t:" opt
do
   case "$opt" in
      n ) times="$OPTARG" ;;
      t ) threads="$OPTARG" ;;
      ? ) helpFunction ;; # Print helpFunction in case parameter is non-existent
   esac
done

if [ -z "$times" ]
then
   echo "Parameters are empty";
fi
run