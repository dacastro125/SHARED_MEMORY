#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


void* funciona_maneja_hilo(void*);
typedef unsigned long int tipo_hilo;

struct nodo{
	int val;
};
pthread_t pidhilo[4];
int main(){
	
	pthread_t pidhilo[4];
	struct nodo *Nodo;
	int i=0;	
	Nodo=(struct nodo *)malloc(sizeof(struct nodo));
	Nodo->val=0;
	pthread_create(&pidhilo[0],NULL,funciona_maneja_hilo,(void*)Nodo);		
	pthread_join(pidhilo[0],NULL);
	printf("Soy el padre con id %lu\n",pthread_self());
	Nodo=(struct nodo *)malloc(sizeof(struct nodo));
	Nodo->val=2;
	pthread_create(&pidhilo[2],NULL,funciona_maneja_hilo,(void*)Nodo);
	pthread_join(pidhilo[2],NULL);
	printf("Soy el padre con id %lu\n",pthread_self());		
	return 0;
	
}

void* funciona_maneja_hilo(void *param){
	struct nodo *Nodo;
	printf("Soy el hijo: %d con id: %lu\n",((struct nodo*)param)->val+1,pthread_self());
	int x = ((struct nodo*)param)->val;
	
	if (x==0)
	{
		Nodo=(struct nodo *)malloc(sizeof(struct nodo));
		Nodo->val=1;
		pthread_create(&pidhilo[1],NULL,funciona_maneja_hilo,(void*)Nodo);
	}
	
	if (x==2)
	{
		Nodo=(struct nodo *)malloc(sizeof(struct nodo));
		Nodo->val=3;
		pthread_create(&pidhilo[3],NULL,funciona_maneja_hilo,(void*)Nodo);
	}
		
	pthread_exit(0);
}
