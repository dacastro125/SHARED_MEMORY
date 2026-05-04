#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//#define N 4;
int n;
int **A, **B, **C;
pthread_t hiloid[4];

void *funcionOperaThread(void *param);

int main(){
    int i,j,*a;    
 
 
    printf("Escriba el tamaño de la matriz...");
    scanf("%d",&n);
 
    A=(int**)malloc(n*sizeof(int*));
    for(i=0;i<n;i++){
        A[i]=(int*)malloc(n*sizeof(int));
    }
 
    B=(int**)malloc(n*sizeof(int*));
    for(i=0;i<n;i++){
        B[i]=(int*)malloc(n*sizeof(int));
    }
 
    C=(int**)malloc(n*sizeof(int*));
    for(i=0;i<n;i++){
        C[i]=(int*)malloc(n*sizeof(int));
    }
 
    printf("ingrese los datos para la matriz 1...");
    for(i=0;i<n;i++)
        for(j=0;j<n;j++){
            printf("M1[%d][%d]: ",i,j);
            scanf("%d",&A[i][j]);
        }
    
    printf("ingrese los datos para la matriz 2...");
    for(i=0;i<n;i++)
        for(j=0;j<n;j++){
            printf("M2[%d][%d]: ",i,j);
            scanf("%d",&B[i][j]);
        }
 
    for(i=0;i<4;i++){
        a=(int*)malloc(sizeof(int));
        *a=i+1;
        
        pthread_create(&hiloid[i],NULL,run,(void*)a);
    }
 
    for(i=0;i<4;i++){        
        pthread_join(hiloid[j],NULL);
    }
    return 0;    
}

void *funcionOperaThread(void *param){
    int *a = (int*)param;/*se recive el argumento*/
 
    int i,j,k,r;
 
        for(k=0;k<n;k++){        
            for(i=0;i<n;i++){
                for(j=0;j<n;j++){
                    Mr[k][i+b]+=M1[k][j]*M2[j][i+b];
                    
                }
            }
 
    



 
        /*Imprime el resultado*/
        printf("\nMATRIZ RESULTANTE:\n");
        for(i=0;i<n;i++){
            printf("|\t");
            for(j=0;j<n;j++){
                printf("%d\t",C[i][j]);
            }
            printf("|\n");
        }
    }
    pthread_exit(NULL);/*Independiemte mente de quien seas... sal de la función*/
}
