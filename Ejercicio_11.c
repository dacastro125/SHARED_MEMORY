#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

void* function_maneja_hilo(void *);
typedef unsigned long int tipo_hilo;

struct nodo{
	int val;
};

int main(){
	pthread_t pidhilo[2];
	struct nodo *Nodo;
	int i;
	for(i = 0; i < 2; i++){
		Nodo = (struct nodo *)malloc(sizeof(struct nodo));
		Nodo->val = i;
		pthread_create(&pidhilo[i], NULL, function_maneja_hilo, (void*) Nodo);	
	}
	printf("Hilo principal (idthread->[%ld])\n", (long int) pthread_self());
	for(i = 0; i < 2; i++){
		pthread_join(pidhilo[i], NULL);
	}
	return 0;
}

void* function_maneja_hilo(void *param){
	printf("Hilo param->%d principal (idthread->[%ld])\n", ((struct nodo *)param)->val, pthread_self());
	free(param);
	pthread_exit(0);
}