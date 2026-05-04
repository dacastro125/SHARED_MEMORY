#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void* funcion_maneja_hilo (void *);
typedef unsigned long int tipo_hilo;

int turno;
struct nodo{
	int val, turnoL;
	
};

int main (){
	pthread_t pidhilo[2];
	struct nodo *Nodo;
	int i;
	turno = -1;
	for (i=0; i<3; i++){
	Nodo = (struct nodo *)malloc(sizeof(struct nodo));
	//Nodo->val = i;
	Nodo->turnoL =i;
	pthread_create(&pidhilo[i], NULL, funcion_maneja_hilo, (void*) Nodo);
	//Nodo->turnoG++;
	}
	printf ("Hilo principal (idthread -> [%lu])\n", pthread_self());
        turno++;
	for (i=0; i<3; i++){
	pthread_join(pidhilo[i], NULL);
	}
return 0;
}

void* funcion_maneja_hilo(void* param){
	int turnoL=((struct nodo *) param)->turnoL;
	while (turnoL!=turno);
	printf ("Hilo %d (idthread-> [%lu])\n", ((struct nodo *) param)->val, pthread_self());
	turno++;
	//pthread_exit(0);
	
}
