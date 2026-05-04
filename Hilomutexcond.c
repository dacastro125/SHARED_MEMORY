#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

void*function_hilo(void *param);
void algo(int);
int turno=0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond= 	PTHREAD_COND_INITIALIZER;


int main (){

	int nhilos=0,i=0;
	pthread_t *pidhilos=NULL;
	printf("Numero de hilos");
	scanf("%d",&nhilos);

	pidhilos=(pthread_t*)calloc(nhilos,sizeof(pthread_t));
	for ( i = 0; i < nhilos; i++)
	{
		pthread_create(&pidhilos[i],NULL,function_hilo,(void*)i);
	}
 	for ( i = 0; i < nhilos; i++)
 	{
 		pthread_join(pidhilos[i],NULL);
 	}
return 0;
}

void* function_hilo(void*param){
  int myturno= (int)param;
  pthread_mutex_lock(&mutex);
  while(turno != myturno){
  	pthread_cond_wait(&cond,&mutex);
  }
  printf("Hilo turno %d\t[%u]\n",myturno,(unsigned int)pthread_self() );
  turno++;
  pthread_cond_broadcast(&cond);

  pthread_mutex_unlock(&mutex);
  pthread_exit(0);
}