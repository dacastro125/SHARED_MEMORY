#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* funcion_maneja_hilo(void *);
typedef unsigned long int tipo_hilo;

struct nodo{
	int val;
};

int main(){
	pthread_t pidhilo[4];
	struct nodo Nodo;
	
	int i; 

		printf("hilo principal (idthread -> [%lu])\n",  pthread_self());
	for(i=0; i<4; i++){

		Nodo.val=i;
		pthread_create(&pidhilo[i], NULL, funcion_maneja_hilo, (void*)&Nodo);

		}

		
		for(i=0; i<4; i++){

		pthread_join(pidhilo[i], NULL);		

		}
		printf("hilo principal (idthread -> [%lu])\n",  pthread_self());		
return(0);
}

void* funcion_maneja_hilo(void *param){
	printf("hilo param -> %d (idthread -> [%lu])\n", ((struct nodo *) param) ->val, pthread_self());


	pthread_exit(0);
}
