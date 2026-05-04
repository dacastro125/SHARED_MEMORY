#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

int **A,**B,**R; // matrices a  atilizar
pthread_t *h; // vector de hilos a crear
int N; // tamaño de la matrix


void* f(void *arg){
	int col = *(int*)arg;
	int i,j;

	if( col<N ){
	 
	     for( i=0; i<N; i++ ){
	     	R[i][col] = 0;
	     	for( j=0; j<N; j++ ){
	     	   R[i][col] = R[i][col] + (A[i][j]*B[j][col]);
		       
			}
		 }	


	}
	else{
		for( i=0; i<N; i++ ){
			pthread_join(h[i],NULL);
		}
		
		printf("\n");
		for( i=0; i<N; i++ ){
        	for( j=0; j<N; j++ ){
                 printf("\t%d",R[i][j]);
		     }
		     printf("\n");
		}
		printf("\n");
	}
	
	return NULL;
}

int main(){
	
	int i,j;
	
	printf("Ingrese el tamaño de matrix N*N .:. ");
	scanf("%d",&N);
//----------------------------------------------------
//Creamos la matriz dinamicamente	
	A = (int**)calloc(N,sizeof(int*));
    B = (int**)calloc(N,sizeof(int*));
	R = (int**)calloc(N,sizeof(int*));	
  
   for( i=0; i<N; i++ ){
   	   A[i] = (int*)calloc(N,sizeof(int));
   	   B[i] = (int*)calloc(N,sizeof(int));
   	   R[i] = (int*)calloc(N,sizeof(int));
   }	
	
   h = (pthread_t*)calloc(N+1,sizeof(pthread_t));
 //-----------------------------------------------
 // Lenamos la matriz    
   int d = 1; 
   for( i=0; i<N; i++ ){
       for( j=0; j<N; j++ ){
            A[i][j]=d;
            B[i][j]=d;
            d++;
		}
   }
 //-----------------------------------------------
 //Creamos Hilos dinamicamente  
   int *col;
   for( i=0; i<(N+1); i++ ){
   	 col = (int*)calloc(1,sizeof(int));
   	 *col = i;
   	 pthread_create(&h[i],NULL,f,(void*)col);
   }
   
    pthread_join(h[N],NULL);
    
	return 0;
}


