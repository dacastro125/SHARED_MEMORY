#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void* function_maneja_hilo(void *);

int main(){
	pthread_t pidhilo;
	pthread_create(&pidhilo, NULL, function_maneja_hilo, NULL);
	printf("Hilo principal (idthread->[%ld])\n", (long int) pthread_self());
	pthread_join(pidhilo, NULL);
	return 0;
}

void* function_maneja_hilo(void *param){
	printf("Hilo principal (idthread->[%ld])\n", (long int) pthread_self());
	pthread_exit(0);
}