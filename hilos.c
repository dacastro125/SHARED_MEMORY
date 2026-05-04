#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

void *funcion_hilo(void* param);
void algo(int);
int sum;

main(){

	int i;
	pthread_t thread_id;
	
	sum=0;
	printf("Previa Creación de Hilos sum=%d\n", sum);

	pthread_create(&thread_id, NULL, funcion_hilo, NULL);
	for(i=0 ; i<10000 ; i++){
		algo(16);
		sum++ ;	
	}

	pthread_join(thread_id, NULL);
	printf("Hilo principal sum=%d\n", sum);
	return 0;

}

void *funcion_hilo(void *param){
	int i;
	printf("Hilos %lu \n", pthread_self());

	for(i=0; i<10000 ; i++){
	sum ++;
	algo(16);
	}

	pthread_exit(0);

}

void algo(int n){
	usleep(n);
}


