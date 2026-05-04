#include<stdio.h>
#include<pthread.h>

void *funcion_hilo(void * param);
void algo(int);
int sum;
pthread_mutex_t mutex= PTHREAD_MUTEX_INITIALIZER;

main(){

	int i;
	pthread_t thread_id;


	sum=0;

	printf("Previa creacion de hilos sum=%d\n",sum);

	pthread_create(&thread_id,NULL, funcion_hilo,NULL);
		for(i=0; i<10000;i++){
			algo(16);
			pthread_mutex_lock(&mutex);
			sum ++;
			pthread_mutex_unlock(&mutex);
		}

	pthread_join( thread_id, NULL);
	printf("Hilo Principal Sum=%d\n", sum);
	pthread_mutex_destroy(&mutex);
	return 0;
}

void * funcion_hilo(void *param){
	int i;
	printf("Hilos %lu\n", pthread_self());
		for(i=0;i<10000; i++){
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





