#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void* funcion_maneja_hilo (void *);
typedef unsigned long int tipo_hilo;

struct nodo{
	int val;
};

int main (){
	pthread_t pidhilo[2];
	struct nodo *Nodo;
	int i;

	for (i=0; i<3; i++){
	Nodo = (struct nodo *)malloc(sizeof(struct nodo));
	Nodo->val = i;
	pthread_create(&pidhilo[i], NULL, funcion_maneja_hilo, (void*) Nodo);
	
	}
	printf ("Hilo principal (idthread -> [%lu])\n", pthread_self());
	for (i=0; i<3; i++){
	pthread_join(pidhilo[i], NULL);
	}
return 0;
}

void* funcion_maneja_hilo(void* param){
	printf ("Hilo param ->%d (idthread-> [%lu])\n", ((struct nodo *) param)->val, pthread_self());
	pthread_exit(0);
}
