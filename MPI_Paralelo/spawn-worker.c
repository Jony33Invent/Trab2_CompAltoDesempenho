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

int main(int argc, char **argv)  {
    int tag = 0, my_rank, num_proc;
    int Cij;

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
    MPI_Recv(&Cij, 1, MPI_INT, 0, tag, inter_comm, &status);
    printf("worker nr %d (%s): received Cij : %d\n", my_rank, processor_name, Cij);
    
    MPI_Finalize();
    /*
    printf("worker nr %d (%s): master data received novoC\n",my_rank, processor_name);
    for(int i=0;i<5;i++){
        printf("%d\t", novoC[i]);
    }printf("\n");*/
    exit(0);
    
}