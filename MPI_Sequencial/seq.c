#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include"mpi.h"

# define MAX 370000

// calculo das permutacoes das N cidades
int fatorial(int N) {
  int res = 1;
  for (int i = 2; i <= N; i++) {
    res = res * i;
  }
  return res;
}

// geracao pseudo-aleatorio dos custos das distancias entre as cidades i e j
int *gerarPesos(int N) {
  srand(32);
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

// impressao da matriz de distancia
void mostraMatrizDistancias(int N, int *dist) {
  printf("Matriz de distancias:\n");
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      printf("%d\t", dist[N*i+j]);
    }
    printf("\n");
  }
}

// funcao auxiliar para troca de valores
void swap(int *x, int *y) {
  char temp;
  temp = *x;
  *x = *y;
  *y = temp;
}

int perm[MAX][11];
int permutar(int *a, int l, int r,int j, int size) {
  int i;
  if (l == r) {
    for (int i = 0; i <size; i++) {
      perm[j][i]=a[i];
    }
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

// calcula custo entre 2 cidades
int custo(int*dist, int* cam, int ini, int n){
  int N=n+1;
  int custo=dist[ini*N+cam[0]];
  for(int i=0;i<n-1;i++){
    custo+=dist[cam[i]*N+cam[i+1]];
  }
  custo+=dist[cam[n-1]*N+ini];
  return custo;
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

  //mostraMatrizDistancias(N, dist);

  printf("Total de caminhos = %d\n", qntCaminhos);
  permutar(num, 0, N-2,0,N-1);
  double t1, t2; 
  t1 = MPI_Wtime(); 
  for(int i=0;i<qntCaminhos;i++){
    c=custo(dist,perm[i],ini,N-1);
    if(c<minimo){
      minimo=c;
      minId=i;
    }
  }

  t2 = MPI_Wtime(); 

  printf("Caminho minimo: ");
    printf("%d\t",ini);
  for(int i=0;i<N-1;i++)
    printf("%d\t",perm[minId][i]);
  printf("%d\t",ini);
  printf("\nCusto %d\n",minimo);

  printf( "Time: %f\n", t2 - t1 ); 
  return 0;
}