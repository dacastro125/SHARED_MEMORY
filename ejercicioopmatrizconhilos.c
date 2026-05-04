#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t myCond = PTHREAD_COND_INITIALIZER;

void * funcionhilo(void *);

int turno =0;
int matrix [3][3]={{0, 1, 2}, {3, 0, 3}, {1, 1, 2}}; 

int main()
{
	int  i=0, j=0, k=0;
    int valor = 0;
    pthread_t *pidhilo=NULL;
    pidhilo = (pthread_t *) calloc(3, sizeof(pthread_t));
    
    for ( i = 0; i < 3; i++)pthread_create(&pidhilo[i], NULL, funcionhilo,(void*)i);
           
    
    for(i=0;i<3;i++) pthread_join(pidhilo[i],NULL);
      
    free(pidhilo);
    return 0;
   
}

void * funcionhilo(void *param){
	int miTurno = (int)param;
	int  i=0, k=0, j=0;
	int escalar = 0;
	int matrizResult[3][3];
	int vector[3], vectorr[3];
	pthread_mutex_lock(&myMutex);
    while(turno!=miTurno) pthread_cond_wait(&myCond,&myMutex);
    switch (miTurno){
    	case 0:
    			printf("imprimiendo desde el primer hilo la matriz inicial\n");
    			for (i = 0; i < 3; i++)
    			{
    				printf("[");
    				for ( j = 0; j < 3; j++)
    				{
    					printf(" %d ",matrix[i][j] );
    				}
    				printf("]\n");
    			}
    			break;
    	case 1:
	    		printf("Multiplicar una matriz por un escalar... por favor ingrese el número a multiplicar la matriz: ");
	    		scanf("%d",&escalar);
	    		printf("\nMatriz resultante de la multiplicación por el escalar %d desde el segundo hilo\n",escalar );
	    		for (i = 0; i < 3; i++)
    			{	   
    				printf("[");
    				for ( j = 0; j < 3; j++)
    				{
    					matrizResult[i][j] = matrix[i][j]*escalar;
    					printf(" %d ",matrizResult[i][j]);
    				}
    				printf("]\n");
    				
    			}
    			break;
    	case 2:
				printf("Multiplicar una matriz por un vector.. hilo 3 ejecutandose...\n");
				for (i = 0; i < 3; i++)
				{
					printf("\nPor favor ingrese el valor en la posición %d: ",i );
					scanf("%d",&vector[i]);
				}
	    		printf("\nMatriz resultante de la multiplicación por el vector ingresado...\n");

	    		for (i = 0; i < 3; i++)
    			{	   
    				printf("[");
    				vectorr[i]=0;
    				for ( j = 0; j < 3; j++)
    				{
    					vectorr[i] += matrix[i][j]*vector[j];
    					
    				}
    				printf("%d]\n",vectorr[i]);
    				
    			}
    			break;
    			default:
    				printf("FIn de la ejecución con Hilos....\n");

    }
    turno++;
  	pthread_cond_broadcast(&myCond);
    pthread_mutex_unlock(&myMutex);
    usleep(1000);
    pthread_exit(0);
}
