#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void* funcion_maneja_hilo(void *);
typedef unsigned long int tipo_hilo;

struct nodo
{
	int val;
};

int main(){
	pthread_t pidhilo;
	struct nodo Nodo;
	Nodo.val = 5;
	pthread_create(&pidhilo, NULL, funcion_maneja_hilo, (void *) &Nodo);
	printf("Hilo principal (idthread -> [%lu])\n", pthread_self());
	pthread_join(pidhilo, NULL);
	return 0;
}

void* funcion_maneja_hilo(void *param){
	printf("Hilo param -> %d (idthread -> [%lu])\n", ((struct nodo *) param)->val, pthread_self());
	pthread_exit(0);
}
