#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void * funhilos(void *);
int turno = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cond  = PTHREAD_COND_INITIALIZER;


int main(){
	int nhilos = 0, i=0;
	pthread_t *pidhilos = NULL;
	printf("Numero de hilos\n");
	scanf("%d", &nhilos);
		
	//creacion de hilos
	pidhilos = (pthread_t *) calloc(nhilos, sizeof(pthread_t));
	for(i=0; i<nhilos; i++)
		pthread_create(&pidhilos[i], NULL, funhilos, (void*)i);
	for(i=0; i<nhilos; i++)
		pthread_join(pidhilos[i], NULL);

	
	free(pidhilos);
	return 0;
}

void * funhilos( void *arg){
	int myturno = (int)arg;
	printf(" numero del hilo: %d\n",myturno);
	
	
	pthread_mutex_lock(&mutex);
	while(turno != myturno) pthread_cond_wait(&cond, &mutex);	

	printf("Hilo turno %d\t[%u]\n", myturno, (unsigned int)pthread_self());
	turno ++;

	pthread_cond_broadcast(&cond);
	pthread_mutex_unlock(&mutex);

	pthread_exit(0);
}