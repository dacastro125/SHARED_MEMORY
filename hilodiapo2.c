#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void* funcion_maneja_hilo(void *);

int main(){
	pthread_t pidhilo;
	pthread_create(&pidhilo, NULL, funcion_maneja_hilo, "hola");
	printf("Hilo principal (idthread -> [%ld])\n", (long int) pthread_self());
	pthread_join(pidhilo, NULL);
	return 0;
}

void* funcion_maneja_hilo(void *param){
	printf("Hilo param -> %s (idthread -> [%ld])\n", (char *) param, (long int) pthread_self());
	pthread_exit(0);
}
