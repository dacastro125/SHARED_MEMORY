#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#define N 256

void *funcion_hilo(void * );


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int i, n, turno=0;

struct nodo{
	int h;	
};



int main(){
	
	pthread_t hilos[N];	
	struct nodo *Nodo;
	printf("ingrese el numero de hilos :");
	scanf("%d",&n);
	for(i=0; i<n; i++){
		Nodo = (struct nodo *)malloc(sizeof(struct nodo));
		pthread_mutex_lock(&mutex);
		Nodo->h=i;
		pthread_create (&hilos[i], NULL, funcion_hilo,(void*) &Nodo->h);

		pthread_mutex_unlock(&mutex);
	}

	for(i=0; i<n; i++){
	pthread_join (hilos[i], NULL);
	}



return 0;	
}


void * funcion_hilo(void *param)
{

	
	int i;
	i=*(int*) param;
	while(i!=turno);
	printf("Hilos %d--%lu \n", i,pthread_self());
	
	pthread_mutex_lock(&mutex);
	turno++;
	pthread_mutex_unlock(&mutex);
	  
}