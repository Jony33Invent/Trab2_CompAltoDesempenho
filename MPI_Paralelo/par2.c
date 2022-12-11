#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <string.h>
#include"mpi.h"
# define MAX 10000
int fatorial(int N) {
  int res = 1;
  for (int i = 2; i <= N; i++) {
    res = res * i;
  }
  return res;
}

int *geraPesos(int N) {
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

void troca(int *x, int *y) {
  char temp;
  temp = *x;
  *x = *y;
  *y = temp;
}

int perm[MAX][15];
int permutacao(int *a, int l, int r,int j, int size) {
  int i;
  if (l == r) {
    for (int i = 0; i <size; i++) {
      perm[j][i]=a[i];
    }
    return j+1;
  } else {
    for (i = l; i <= r; i++) {
      troca((a + l), (a + i));
      j=permutacao(a, l + 1, r,j,size);
      troca((a + l), (a + i));
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


  // váriaveis MPI
  int  tag = 1, rank, num_proc, src, dst, root;
  char worker[40];
  int  errcodes[10], i, k, j;

  char processor_name[MPI_MAX_PROCESSOR_NAME];
  int name_len;

  

    MPI_Status    status;
    MPI_Comm      inter_comm;
    MPI_Request   mpirequest_mr;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_proc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Get_processor_name(processor_name, &name_len);


    int N = atoi(argv[1]);
    int n=N-1;


    int *dist = geraPesos(N); // matriz de distancias entre vértices

    int ini=0;
    int *num=geraNum(N);  // numero dos vertices a percorrer


    double t1, t2; 
    t1 = MPI_Wtime(); 
    src = dst = root = 0;

    if(rank==0){

      int qntCaminhos = fatorial(N - 1);
      int minimo=INT_MAX;

      int count=qntCaminhos/num_proc;
      int sobra=qntCaminhos%num_proc;
  
      mostraMatrizDistancias(N, dist);
      printf("Total de caminhos = %d\n", qntCaminhos);
      permutacao(num, 0, n-1,0,n);

      j=0;
      // envia valores da permutação de caminhos para trabalhadores
      for (i = 1; i < num_proc; i++){
        for(k=0;k<count;k++,j++){
          int caminhoAtual[n];
          for(int m=0;m<n;m++){
            caminhoAtual[m]=perm[j][m];
          }
            MPI_Send(caminhoAtual, n, MPI_INT, i, tag, MPI_COMM_WORLD);
        }
      }
       // envia o que sobrou
      i=num_proc-1;
      for(k=0;k<sobra;k++,j++){
        int caminhoAtual[n];
        for(int m=0;m<n;m++){
          caminhoAtual[m]=perm[j][m];
        }
        MPI_Send(caminhoAtual, n, MPI_INT, i, tag, MPI_COMM_WORLD);
      }
      int c;
      for (i = 1; i < num_proc; i++){
        MPI_Recv(&c,1,MPI_INT,i,tag,MPI_COMM_WORLD, &status);
        if(c<minimo)
          minimo=c;
        
      }
      printf("\nCusto %d\n",minimo);
    }
    else{
      int caminho[N-1];
      // recebe caminho e calcula o custo
      printf("nr %d iniciou\n",rank);
      MPI_Recv(caminho, N-1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
      int c=custo(dist,caminho,ini,N-1);
      printf("nr %d recebeu caminho com custo %d\n",rank,c);
      // calcula custo e envia de volta para o mestre
      c=custo(dist,perm[i],ini,N-1);
      MPI_Send(&c, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
    }
    t2 = MPI_Wtime(); 
    MPI_Finalize();
    exit(0);


  return 0;
}