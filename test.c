#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

int custo = 0;

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
int *geraDist(int N) {
  srand(time(NULL));
  int *dist = (int *)malloc(N *N* sizeof(int *));
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (i == j)
        dist[i*N+j] = 0;
      else
        dist[i*N+j] = rand() % 100;
    }
  }
  return dist;
}

void printGraph(int N, int *dist) {
  printf("Grafo Gerado:\n");
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      printf("%d\t", dist[i*N+j]);
    }
    printf("\n");
  }
}

int tsp(int atual, int *dist, int *vis, int N) {
  int i, noMaisPerto = MAX_INT;
  int min = MAX_INT, temp;
  for (i = 0; i < N; i++) {
    if ((dist[atual*N+i] != 0) && (vis[i] == 0)) {
      if (dist[atual*N+i] < min) {
        min = dist[i*N] + dist[atual*N+i];
      }
      temp = dist[c*N+i];
      noMaisPerto = i;
    }
  }
  if (min != MAX_INT) 
    custo += temp;
  
  return noMaisPerto;
}

void minimum_cost(int noAtual, int *dist, int *vis, int N) {
  int noMaisPerto;
  vis[noAtual] = 1;
  noMaisPerto = tsp(noAtual, dist, vis, N);
  if (noMaisPerto == MAX_INT) {
    noMaisPerto = 0;
    printf("%d", noMaisPerto + 1);
    custo+= dist[noAtual*N+noMaisPerto];
    return;
  }
  minimum_cost(noMaisPerto, dist, vis, N);
}

int main(int argc, char *argv[]) {
  int i, j;
  int N = atoi(argv[1]);
  int *dist = geraDist(N);
  int *vis = (int *)malloc(N * sizeof(int));
  printGraph(N, dist);
  printf("Menor caminho:\t");
  minimum_cost(0, dist, vis, N);
  printf("\nMenor Custo:\t");
  printf("%d\n", cost);
  return 0;
}