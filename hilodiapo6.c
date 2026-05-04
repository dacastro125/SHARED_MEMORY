#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void* funcion_hilo(void * param);
void algo(int );
int sum;


int main(){
	int i, NumHilos=5;
	pthread_t * thread_id;
	sum=0;
	printf("Previa creacion de hilos sum=%d\n", sum);
	thread_id = (pthread_t*)calloc(NumHilos, sizeof(pthread_t) );
	for (i = 0; i < NumHilos; i++)
	{
		pthread_create(&thread_id[i], NULL, funcion_hilo,NULL);
	}

	for (i = 0; i < NumHilos; i++)
	{
		pthread_join(thread_id[i], NULL);
	}
	printf("Hilo principal sum=%d\n", sum);
	return 0;
}


void* funcion_hilo(void *param){
	int i;
	for(i=0; i<100000; i++){
		sum = sum + 1;
		algo(1);
	}
}

void algo(int n){
	usleep(n);
}
