// para compilar: mpicc 02-spawn-worker.c -o 02-spawn-worker -Wall
// para rodar: sera inciado pelo mestre com MPI_Comm_spawn
/*
!! a simple/Master worker (dynamically spawnd) example with 
!!  MPI_Comm_spawn, MPI_Comm_get_parent
!!  both Master and worker executables must be build, see Master  
*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"mpi.h"

int custo(int*dist, int* cam, int ini, int n){
  int N=n+1;
  int custo=dist[ini*N+cam[0]];
  for(int i=0;i<n-1;i++){
    custo+=dist[cam[i]*N+cam[i+1]];
  }
  custo+=dist[cam[n-1]*N+ini];
  return custo;
}
int fatorial(int N) {
  int res = 1;
  for (int i = 2; i <= N; i++) {
    res = res * i;
  }
  return res;
}
# define N 4
int main(int argc, char **argv)  {
    int tag = 0, my_rank, num_proc;
    int caminho[fatorial(N)];

    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;

    MPI_Status      status;
    MPI_Comm        inter_comm; 


    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_proc);
    MPI_Get_processor_name(processor_name, &name_len);


    MPI_Comm_get_parent(&inter_comm);

    printf("worker nr %d\n",my_rank);
    MPI_Recv(caminho, N-1, MPI_INT, 0, tag, inter_comm, &status);
    //MPI_Br
    printf("worker nr %d (%s): received caminho",my_rank,processor_name);
    //int c=custo()
    MPI_Finalize();
    /*
    printf("worker nr %d (%s): master data received novoC\n",my_rank, processor_name);
    for(int i=0;i<5;i++){
        printf("%d\t", novoC[i]);
    }printf("\n");*/
    exit(0);
    
}