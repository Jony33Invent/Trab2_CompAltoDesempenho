# Compilador de c
CC = gcc
# Tamanho do grafo
N = 15
# Número de processos
P = $(shell nproc)
# P = N
# Lista de Hosts
HOST_LIST = -H hal02,hal03,hal04,hal05,hal06,hal07,hal08,hal09,hal10
WARNING_FLAGS = -Wextra -Wall

seq:
  mpicc  -fopenmp ./seq.c -o seq
run-seq: seq
  mpirun -np 1 -H hal02,hal03,hal04,hal05,hal06,hal07,hal08,hal09,hal10 ./seq 4
worker:
  mpicc -fopenmp ./spawn-worker.c -o spawn-worker
par:
  mpicc  -fopenmp ./par.c -o par
run-par: par
  mpirun -np 4 -H hal02,hal03,hal04,hal05,hal06,hal07,hal08,hal09,hal10 --oversubscribe ./par 4