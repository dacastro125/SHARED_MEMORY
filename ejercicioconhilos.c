#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_con_t myCond = PTHREAD_COND_INITIALIZER;

void *funcionhilo(void *);

int turno =0;
struct nodo {
	int val;
	int turnoNodo;
};

void main(){
	pthread_t *pidhilo=NULL;
	struct nodo *Nodo;
	int opcion=0;
	int valor = 0;
	pidhilo = (pthread_t *) calloc(3, sizeof(pthread_t));
	
	return 0;
}

void *funcionhilo(void *param){

}



