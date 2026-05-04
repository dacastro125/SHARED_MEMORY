#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int **MatrizA,**MatrizB,**MatrizR; //Matrices a  atilizar
pthread_t *hilo; //Vector de hilos a crear
int n; //Tamaño de la Matriz


void* funcion_Matriz(void *arg){
	int col = *(int*)arg;
	int i, j;

	if( col < n ){
	 	MatrizR[i][col] = 0; // es aqui o la de abajo, PROBAR
	     for( i = 0; i < n; i++ ){
	     	//MatrizR[i][col] = 0;
	     	for( j = 0; j < n; j++ ){
	     	   MatrizR[i][col] = MatrizR[i][col] + (MatrizA[i][j]*MatrizB[j][col]);
		       
			}
		 }	

	}
	else{
		for( i = 0; i < n; i++ ){
			pthread_join(hilo[i],NULL);
		}
		
		printf("\n\t La Matriz RESULTANTE es: \n\n");
		printf("\n");
		for( i = 0; i < n; i++ ){
        	for( j = 0; j < n; j++ ){
                 printf("\t %d",MatrizR[i][j]);
		     }
		     printf("\n");
		}
		printf("\n");
	}
	
	return NULL;
}

int main(){

    srand(time(NULL));

	int i, j;
	
	printf("\n Ingrese el tamaño de la Matrix NxN : ");
	scanf("%d",&n);
	
	MatrizA = (int**)calloc(n,sizeof(int*));
    MatrizB = (int**)calloc(n,sizeof(int*));
	MatrizR = (int**)calloc(n,sizeof(int*));	
  
   for( i = 0 ; i < n; i++ ){
   	   MatrizA[i] = (int*)calloc(n,sizeof(int));
   	   MatrizB[i] = (int*)calloc(n,sizeof(int));
   	   MatrizR[i] = (int*)calloc(n,sizeof(int));
   }	
	
   hilo = (pthread_t*)calloc(n+1,sizeof(pthread_t));
    
   
   for( i = 0; i < n; i++ ){
       for( j = 0; j < n; j++ ){
            MatrizA[i][j] = rand()%9;
            MatrizB[i][j] = rand()%9;
       }
   }
   funcion_imprimir_MatrizA();
   funcion_imprimir_MatrizB();
   int *col;
   for( i = 0; i < (n+1); i++ ){
   	 col = (int*)calloc(1,sizeof(int));
   	 *col = i;
   	 pthread_create(&hilo[i],NULL,funcion_Matriz,(void*)col);
   }
   
    pthread_join(hilo[n],NULL);
    
	return 0;
}

funcion_imprimir_MatrizA(){
	int i, j;
	printf("\n\t La matriz A es: \n\n");
	for( i = 0; i < n; i++ ){
			pthread_join(hilo[i],NULL);
		}
		
		printf("\n");
		for( i = 0; i < n; i++ ){
        	for( j = 0; j < n; j++ ){
                 printf("\t %d",MatrizA[i][j]);
		     }
		     printf("\n");
		}
		printf("\n");
}

funcion_imprimir_MatrizB(){
		int i, j;
	
	printf("\n\t La matriz B es: \n\n");
	for( i = 0; i < n; i++ ){
			pthread_join(hilo[i],NULL);
		}
		
		printf("\n");
		for( i = 0; i < n; i++ ){
        	for( j = 0; j < n; j++ ){
                 printf("\t %d",MatrizB[i][j]);
		     }
		     printf("\n");
		}
		printf("\n");
}

