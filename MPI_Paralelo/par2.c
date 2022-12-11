#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <string.h>
#include"mpi.h"
# define MAX 50000
#define NUM_SPAWNS 4
int fatorial(int N) {
  int res = 1;
  for (int i = 2; i <= N; i++) {
    res = res * i;
  }
  return res;
}

int *gerarPesos(int N) {
  srand(time(NULL));
  int *dist = (int *)malloc(N*N * sizeof(int *));
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (i == j)
        dist[N*i+j] = 0;
      else
        dist[N*i+j] = rand() % 10;
    }
  }
  return dist;
}

void mostraMatrizDistancias(int N, int *dist) {
  printf("Matriz de distancias:\n");
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      printf("%d\t", dist[N*i+j]);
    }
    printf("\n");
  }
}

void swap(int *x, int *y) {
  char temp;
  temp = *x;
  *x = *y;
  *y = temp;
}

int perm[MAX][15];
int permutar(int *a, int l, int r,int j, int size) {
  int i;
  if (l == r) {
    //printf("j: %d\n",j);
    for (int i = 0; i <size; i++) {
      //printf("%d", a[i]); 
      perm[j][i]=a[i];
    }
    //printf("\n");
    return j+1;
  } else {
    for (i = l; i <= r; i++) {
      swap((a + l), (a + i));
      j=permutar(a, l + 1, r,j,size);
      swap((a + l), (a + i)); // backtrack
    }
  }
  return j;
}
int custo(int*dist, int* cam, int ini, int n){
  int N=n+1;
  int custo=dist[ini*N+cam[0]];
  for(int i=0;i<n-1;i++){
    custo+=dist[cam[i]*N+cam[i+1]];
  }
  custo+=dist[cam[n-1]*N+ini];
  return custo;
}
int* teste(int N){
  int test[] = {1, 2, 2, 5, 1, 4, 3, 8, 1, 1, 7, 6};
  int k = 0;
  int *dist = (int *)malloc(N*N * sizeof(int));
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (i == j)
        dist[N*i+j] = 0;
      else
        dist[N*i+j] = test[k++];
    }
  }
  return dist;
}

int* geraNum(int N){
 int* num=(int*)malloc(sizeof(int)*N);
  for(int i=1;i<N;i++)
    num[i-1]=i;
  return num;
}
int main(int argc, char *argv[]) {
  int N = atoi(argv[1]);
  int *dist = gerarPesos(N);
  int *num=geraNum(N);
  int qntCaminhos = fatorial(N - 1);
  int ini=0;
  int minimo=INT_MAX;
  int c;
  int minId;


  // váriaveis MPI
  int  tag = 1, my_rank, num_proc, src, dst, root;
  char worker[40];
  int  errcodes[10], i, k, j;

  char processor_name[MPI_MAX_PROCESSOR_NAME];
  int name_len;

  int n=N-1;
  mostraMatrizDistancias(N, dist);
  printf("Total de caminhos = %d\n", qntCaminhos);
  permutar(num, 0, n-1,0,n);




    strcpy(worker,"worker");
  
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_proc);
  
    MPI_Get_processor_name(processor_name, &name_len);
    int count=qntCaminhos/NUM_SPAWNS;
    int sobra=qntCaminhos/NUM_SPAWNS;
    double t1, t2; 
    t1 = MPI_Wtime(); 
    j=0;

    src = dst = root = 0;
    MPI_Comm_spawn(worker, MPI_ARGV_NULL, NUM_SPAWNS, MPI_INFO_NULL, root, MPI_COMM_WORLD, &inter_comm, errcodes);
    printf("spawnou trabalhadores\n");
    // envia valores da permutação para trabalhadores
    for (i = 0; i < NUM_SPAWNS; i++){
      for(k=0;k<count;k++,j++){
          MPI_Send(perm[j], n, MPI_INT, i, tag, inter_comm);
          MPI_Send(dist, N*N, MPI_INT, i, tag, inter_comm);
      }
    }

    t2 = MPI_Wtime(); 
    MPI_Finalize();
    exit(0);

  printf("\nCusto %d\n",minimo);

  return 0;
}