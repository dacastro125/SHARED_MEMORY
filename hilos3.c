#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

void* funcion_maneja_hilo(void*);
typedef unsigned long int tipo_hilo;

struct nodo{
	int val;
};

int main(int argc, char const *argv[])
{
	int i;
	pthread_t pidhilo[10];
	struct nodo *Nodo;
	
	printf("Hilo principal(idthread -> [%ld])\n", (long int) pthread_self());
	for (i = 0; i < 10; i++){
		Nodo = (struct nodo *)malloc(sizeof(struct nodo));//tienen direcciones diferentes dependiendo de la iteracion
		Nodo->val = i;
		pthread_create(&pidhilo[i], NULL, funcion_maneja_hilo, (void*) Nodo);// no se le manda la cadena hola, 
	}
	for (i = 0; i < 10; i++){
		pthread_join(pidhilo[i], NULL);
	}
	return 0;
}

void* funcion_maneja_hilo(void *param){// argumento void (sin tipo) para que pueda ser enviado cualquier tipo
	printf("hilo param -> %d\t\t(idthread ->[%lu])\n",((struct nodo *)param)->val, (long int) pthread_self());
	pthread_exit(0);
}