#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

int cost = 0;

int **genDist(int N) {
  srand(32);
  int **dist = (int **)malloc(N * sizeof(int *));
  for (int i = 0; i < N; i++) {
    dist[i] = (int *)malloc(N * sizeof(int));
    for (int j = 0; j < N; j++) {
      if (i == j)
        dist[i][j] = 0;
      else
        dist[i][j] = rand() % 100;
    }
  }
  return dist;
}

void printGraph(int N, int **dist) {
  printf("Grafo Gerado:\n");
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      printf("%d\t", dist[i][j]);
    }
    printf("\n");
  }
}

int tsp(int c, int **matrix, int *visited_cities, int limit) {
  int count, nearest_city = 999;
  int minimum = 999, temp;
  for (count = 0; count < limit; count++) {
    if ((matrix[c][count] != 0) && (visited_cities[count] == 0)) {
      if (matrix[c][count] < minimum) {
        minimum = matrix[count][0] + matrix[c][count];
      }
      temp = matrix[c][count];
      nearest_city = count;
    }
  }
  if (minimum != 999) {
    cost = cost + temp;
  }
  return nearest_city;
}

void minimum_cost(int city, int **matrix, int *visited_cities, int limit) {
  int nearest_city;
  visited_cities[city] = 1;
  printf("%d->", city + 1);
  nearest_city = tsp(city, matrix, visited_cities, limit);
  if (nearest_city == 999) {
    nearest_city = 0;
    printf("%d", nearest_city + 1);
    cost = cost + matrix[city][nearest_city];
    return;
  }
  minimum_cost(nearest_city, matrix, visited_cities, limit);
}

int main(int argc, char *argv[]) {
  int i, j;
  int N = atoi(argv[1]);
  int **matrix = genDist(N);
  int *visited_cities = (int *)malloc(N * sizeof(int));
  printGraph(N, matrix);
  printf("Menor caminho:\t");
  minimum_cost(0, matrix, visited_cities, N);
  printf("\nMenor Custo:\t");
  printf("%d\n", cost);
  return 0;
}