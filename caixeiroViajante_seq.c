#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
int min(int a, int b){
    return a>b?b:a;
}
int* removeDoArray(int* arr, int id,int n){
    int* newArr=(int*)malloc(sizeof(int)*(n-1));
    int i=0;
    for(i=0;i<id;i++)
        newArr[i]=arr[i];
    i++;
    for(;i<n;i++){
        newArr[i-1]=arr[i];
    }
    return newArr;
}

int custo(int i, int* C,int n,int** c){
    int minimo=INT_MAX;
    if(n==1){
        return C[0];
    }
    for(int j=0;j<n;j++){
        int Cij=c[i][C[j]];
        int* novoC=removeDoArray(C,j,n);
        minimo=min(minimo,Cij+custo(C[j],novoC,n-1,c));
    }
    return minimo;
}

int main(int argc, char** argv) {
    srand(32);
    int N=4;
    int inicial=0;
    int C[]={1,2,3};
    int test[]={1,2,2,5,1,4,3,8,1,1,7,6};
    int** dist;
    int k=0;
    dist=(int**)malloc(sizeof(int*)*N);
    for(int i=0;i<N;i++){
        dist[i]=(int*)malloc(sizeof(int)*N);
        for(int j=0;j<N;j++){
            if(i!=j){
                // dist[i][j]=rand()%10; //valores aleatórios
                dist[i][j]=test[k++]; // teste do pdf
            }
            else
                dist[i][j]=0;
        }
    }
    
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            printf("%d\t",dist[i][j]);
        }
        printf("\n");
    }
    
    int n=sizeof(C)/sizeof(int);

    
    int pcv=custo(inicial,C,n,dist); // calcula custo de uma viagem 
    
    printf("\nVertíces a percorrer: {\t%d",inicial);
    for(int i=0;i<n;i++){
        printf("\t%d",C[i]);
    }
    printf("\t}\nCusto minimo da viagem: %d\n",pcv);
    return 0;
}