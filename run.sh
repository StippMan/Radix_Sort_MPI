#!/bin/bash

PARALLEL_FILE=./paralelo/full
SEQUENTIAL_FILE=./sequencial/seq
INPUT_FILE=data-set



run(){
    for input in  1 2 3 
        do
            echo "Running sequential algorithm... with = $input elements"
            START=$(date +%s.%N)
                         inputfile= "${INPUT_FILE}${input}.in"
            for ((i=1;i<=times;i++)); do
                $SEQUENTIAL_FILE -f inputfile
            done
            END=$(date +%s.%N)
            TIME_SEQ=$(python3 -c "print('{:.2f}'.format(${END} - ${START}))")
            
         echo "Running paralell algorithm with $procs procs... with = $input elements" 
         START=$(date +%s.%N)
         inputfile= "${INPUT_FILE}${input}.in"
         for ((i=1;i<=times;i++)); do
            mpiexec -n $procs $PARALLEL_FILE -f $inputfile
         done
         END=$(date +%s.%N)
         TIME_PAR=$(python3 -c "print('{:.2f}'.format(${END} - ${START}))")
         SPEEDUP=$(python3 -c "print('{:.2f}'.format(${TIME_SEQ} / ${TIME_PAR}))")
         echo "Speed up for input $input and $procs procs is $SPEEDUP"
         echo ""
        done
}

helpFunction()
{
   echo ""
   echo "Usage: $0 -n "
   echo -e "\t-n number of times that the programas will run "
   echo -e "\t-p number of procs (2 or 4) "
}

while getopts "n:p:" opt
do
   case "$opt" in
      n ) times="$OPTARG" ;;
      p ) procs="$OPTARG" ;;
      ? ) helpFunction ;; # Print helpFunction in case parameter is non-existent
   esac
done

if [ -z "$times" ]
then
   echo "Parameters are empty";
fi
run