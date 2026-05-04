#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void* funcion_maneja_hilo (void *);

int main(){
	pthread_t pidhilo;
	pthread_create(&pidhilo, NULL, funcion_maneja_hilo, NULL);
	printf("Hilo principal (idthread -> [%ld])\n", (long int) pthread_self());
	pthread_join (pidhilo, NULL);
	return 0;
}// fin main

void * funcion_maneja_hilo(void *param){
	printf ("Hilo (idthread -> [%ld])\n",(long int) pthread_self());
	pthread_exit (0);

}//fin funcion
