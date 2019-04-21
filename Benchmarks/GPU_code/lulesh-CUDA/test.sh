#!/bin/bash
#MSUB -l walltime=00:00:05
#MSUB -q pbatch
#MSUB -e elulesh.txt
#MSUB -o olulesh.txt
#MSUB -l nodes=1

make
./lulesh -u sedov15oct.lmesh
