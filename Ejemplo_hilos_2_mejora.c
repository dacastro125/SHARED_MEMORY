#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *funcion_hilo(void*);
void algo(int);
int sum;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main(){
	int i = 0;
	pthread_t idHilo;
	sum = 0;

	printf("Previa creacion de hilos suma = %d\n",sum);

	pthread_create(&idHilo,NULL,funcion_hilo,NULL);
	for(i = 0;i<10000;i++){
		algo(16);
		pthread_mutex_lock(&mutex);
		sum++;
		pthread_mutex_unlock(&mutex);
	}
	pthread_join(idHilo,NULL);
	printf("Hilo principal suma = %d\n",sum);

	return 0;
}

void * funcion_hilo(void *param){
	int i;
	printf("Hilos %lu \n",pthread_self());

	for(i = 0;i<10000;i++){
		pthread_mutex_lock(&mutex);
		sum++;
		pthread_mutex_unlock(&mutex);
		algo(16);
	}
	pthread_exit(0);
}

void algo(int n){
	usleep(n);
}