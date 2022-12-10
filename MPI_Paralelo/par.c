#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define NUM_SPAWNS 4
// Para executar em vários nodes do cluster, rodar, por exemplo:
//  mpicc par.c -o par
//  mpirun -np 1 --hostfile halley.txt par
int min(int a, int b) { return a > b ? b : a; }

int *removeDoArray(int *arr, int id, int n) {
  int *newArr = (int *)malloc(sizeof(int) * (n - 1));
  int i = 0;
  for (i = 0; i < id; i++)
    newArr[i] = arr[i];
  i++;
  for (; i < n; i++) {
    newArr[i - 1] = arr[i];
  }
  return newArr;
}

int custo(int i, int *C, int n, int **c) {
  int minimo = INT_MAX;
  if (n == 1) {
    return C[0];
  }
  for (int j = 0; j < n; j++) {
    int Cij = c[i][C[j]];
    int *novoC = removeDoArray(C, j, n);
    minimo = min(minimo, Cij + custo(C[j], novoC, n - 1, c));
  }
  return minimo;
}

int main(int argc, char **argv) {
  srand(32);
  int N = 4;
  int inicial = 0;
  int C[] = {1, 2, 3};
  // int test[]={1,2,2,5,1,4,3,8,1,1,7,6};
  int *dist;
  int  tag = 1, my_rank, num_proc, src, dst, root;
  char worker[40];
  int  errcodes[10], i, k, j;

  char processor_name[MPI_MAX_PROCESSOR_NAME];
  int name_len;
  dist = (int *)malloc(sizeof(int) * N*N); // matriz de custo das distancias entre arestas 

  // Preenche matriz de custos (dist) das arestas
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (i != j) {
        dist[N*i+j] = rand() % 10; // valores aleatórios
        // dist[i][j]=test[k++]; // teste do pdf
      } else
        dist[N*i+j] = 0;
    }
  }

  // Exibe matriz de custos
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      printf("%d\t", dist[N*i+j]);
    }
    printf("\n");
  }

  int n = sizeof(C) / sizeof(int);


  // Mestre
    
    

    MPI_Status    status;
    MPI_Comm      inter_comm;
    MPI_Request   mpirequest_mr;


    strcpy(worker,"spawn-worker");
  
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_proc);
  
    MPI_Get_processor_name(processor_name, &name_len);
    int count=NUM_SPAWNS/n;
    int sobra=NUM_SPAWNS%n;
    j=0;

    src = dst = root = 0;
    MPI_Comm_spawn(worker, MPI_ARGV_NULL, NUM_SPAWNS, MPI_INFO_NULL, root, MPI_COMM_WORLD, &inter_comm, errcodes);
    printf("spawnou trabalhadores\n");
    int Cij[NUM_SPAWNS][count];
    // envia valores de Cij e caminho para trabalhadores
    for (i = 0; i < NUM_SPAWNS; i++){
      for(k=0;k<count;k++,j++){
          int Cij[i][k] = dist[inicial*N+C[j]];
          //int *novoC = removeDoArray(C, j, n);
          printf("Sending Cij %d to i: %d\n",Cij,i);
          MPI_Send(&Cij, 1, MPI_INT, i, tag, inter_comm);
          printf("Sended Cij %d to i: %d\n",Cij,i);
          //MPI_Send(novoC, n-1, MPI_INT, i, tag, inter_comm);
      }
    }

    MPI_Finalize();
    exit(0);
  
  /*
  int pcv=custo(inicial,C,n,dist); // calcula custo de um


  printf("\nVertíces a percorrer: {\t%d",inicial);
  for(int i=0;i<n;i++){
      printf("\t%d",C[i]);
  }
  printf("\t}\nCusto minimo da viagem: %d\n",pcv);
  */

  return 0;
}